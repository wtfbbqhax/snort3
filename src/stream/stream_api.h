/*
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
 * ** Copyright (C) 2005-2013 Sourcefire, Inc.
 * ** AUTHOR: Steven Sturges
 * **
 * ** This program is free software; you can redistribute it and/or modify
 * ** it under the terms of the GNU General Public License Version 2 as
 * ** published by the Free Software Foundation.  You may not use, modify or
 * ** distribute this program under any other version of the GNU General
 * ** Public License.
 * **
 * ** This program is distributed in the hope that it will be useful,
 * ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 * ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * ** GNU General Public License for more details.
 * **
 * ** You should have received a copy of the GNU General Public License
 * ** along with this program; if not, write to the Free Software
 * ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * */

/* stream_api.h
 *
 * Purpose: Definition of the StreamAPI.  To be used as a common interface
 *          for TCP (and later UDP & ICMP) Stream access for other
 *          preprocessors and detection plugins.
 */

// FIXIT stream_api should not be tied to a particular version of stream

#ifndef STREAM_API_H
#define STREAM_API_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>

#include "sfip/ipv6_port.h"
#include "protocols/packet.h"
#include "flow/flow.h"

#define SSN_MISSING_NONE   0x00
#define SSN_MISSING_BEFORE 0x01
#define SSN_MISSING_AFTER  0x02
#define SSN_MISSING_BOTH   (SSN_MISSING_BEFORE | SSN_MISSING_AFTER)

#define SSN_DIR_NONE 0x0
#define SSN_DIR_CLIENT 0x1
#define SSN_DIR_SENDER 0x1
#define SSN_DIR_SERVER 0x2
#define SSN_DIR_RESPONDER 0x2
#define SSN_DIR_BOTH 0x03

typedef enum {
    STREAM_FLPOLICY_NONE,
    STREAM_FLPOLICY_FOOTPRINT,       /* size-based footprint flush */
    STREAM_FLPOLICY_LOGICAL,         /* queued bytes-based flush */
    STREAM_FLPOLICY_RESPONSE,        /* flush when we see response */
    STREAM_FLPOLICY_SLIDING_WINDOW,  /* flush on sliding window */
#if 0
    STREAM_FLPOLICY_CONSUMED,        /* purge consumed bytes */
#endif
    STREAM_FLPOLICY_IGNORE,          /* ignore this traffic */
    STREAM_FLPOLICY_PROTOCOL,        /* protocol aware flushing (PAF) */
    STREAM_FLPOLICY_FOOTPRINT_IPS,   /* protocol agnostic ips */
    STREAM_FLPOLICY_PROTOCOL_IPS,    /* protocol aware ips */
    STREAM_FLPOLICY_MAX
} FlushPolicy;

#define STREAM_FLPOLICY_SET_ABSOLUTE    0x01
#define STREAM_FLPOLICY_SET_APPEND      0x02

class Flow;

typedef int (*LogFunction)(Flow*, uint8_t **buf, uint32_t *len, uint32_t *type);
typedef void (*LogExtraData)(Flow*, void *config, LogFunction *funcs,
    uint32_t max_count, uint32_t xtradata_mask, uint32_t id, uint32_t sec);

typedef int (*PacketIterator)
    (
     DAQ_PktHdr_t *,
     uint8_t *,  /* pkt pointer */
     void *      /* user-defined data pointer */
    );

typedef int (*StreamSegmentIterator)
    (
     DAQ_PktHdr_t *,
     uint8_t *,  /* pkt pointer */
     uint8_t *,  /* payload pointer */
     uint32_t,   /* sequence number */
     void *      /* user-defined data pointer */
    );

// for protocol aware flushing (PAF):
typedef enum {
    PAF_ABORT,   // non-paf operation
    PAF_START,   // internal use only
    PAF_SEARCH,  // searching for next flush point
    PAF_FLUSH,   // flush at given offset
    PAF_SKIP     // skip ahead to given offset
} PAF_Status;

typedef PAF_Status (*PAF_Callback)(  // return your scan state
    Flow*,     // session pointer
    void** user,           // arbitrary user data hook
    const uint8_t* data,   // in order segment data as it arrives
    uint32_t len,          // length of data
    uint32_t flags,        // packet flags indicating direction of data
    uint32_t* fp           // flush point (offset) relative to data
);

typedef void (*Stream_Callback)(Packet *);

#define MAX_PAF_CB  8  // depends on sizeof(PAF_Map.cb_mask)
#define MAX_EVT_CB 32
#define MAX_LOG_FN 32

// FIXIT to remain part of stream5 this should be pure virtual
// otherwise pull out of stream5 and implement proto specific stuff
// via calls to virtuals provided by stream5 and stream5 sessions
class Stream
{
public:
    Stream();
    ~Stream();

    static Flow* get_session(const FlowKey*);
    static Flow* new_session(const FlowKey*);
    static void delete_session(const FlowKey*);

    static uint32_t get_packet_direction(Packet*);

    /* Stop inspection for session, up to count bytes (-1 to ignore
     * for life or until resume).
     *
     * If response flag is set, automatically resume inspection up to
     * count bytes when a data packet in the other direction is seen.
     *
     * Also marks the packet to be ignored
     */
    static void stop_inspection(Flow*, Packet*, char dir, int32_t bytes, int rspFlag);

    /* Turn off inspection for potential session.
     * Adds session identifiers to a hash table.
     * TCP only.
     */
    int ignore_session(
        snort_ip_p addr1, uint16_t p1, snort_ip_p addr2, uint16_t p2,
        uint8_t proto, char dir, uint32_t ppId);

    /* Resume inspection for session.
     */
    static void resume_inspection(Flow*, char dir);

    /* Drop traffic arriving on session.
     */
    static void drop_traffic(Packet*, Flow*, char dir);

    /* Drop retransmitted packet arriving on session.
     */
    static void drop_packet(Packet*);  // PKT

    /* Flushes the stream on arrival of another packet
     * Side that is flushed is the opposite of the packet.
     */
    static int response_flush_stream(Packet*);  // PKT

    /* Calls user-provided callback function for each packet of
     * a reassembled stream.  If the callback function returns non-zero,
     * iteration ends.
     *
     * Returns number of packets
     */
    static int traverse_reassembled(Packet*, PacketIterator, void* userdata);  // PKT

    /* Calls user-provided callback function for each segment of
     * a reassembled stream.  If the callback function returns non-zero,
     * iteration ends.
     *
     * Returns number of packets
     */
    static int traverse_stream_segments(Packet*, StreamSegmentIterator, void* userdata);  // PKT

    /* Add session alert
     */
    static int add_session_alert(Flow*, Packet*, uint32_t gid, uint32_t sid);

    /* Check session alert
     *
     * Returns
     *     0 if not previously alerted
     *     !0 if previously alerted
     */
    static int check_session_alerted(Flow*, Packet *p, uint32_t gid, uint32_t sid);

    /* Set Extra Data Logging
     *
     * Returns
     *      0 success
     *      -1 failure ( no alerts )
     */
    static int update_session_alert(
        Flow*, Packet *p, uint32_t gid, uint32_t sid,
        uint32_t eventId, uint32_t eventSecond);

    /* Get Flowbits data
     *
     * Returns
     *     Ptr to Flowbits Data
     */
    static StreamFlowData* get_flow_data(Packet*);

    /* Set reassembly flush policy/direction for given session
     *
     * Returns
     *     direction(s) of reassembly for session
     */
    /* Do not attempt to set flush policy to PROTOCOL or PROTOCOL_IPS. */
    static char set_reassembly(Flow*, FlushPolicy, char dir, char flags);

    /* Get reassembly direction for given session
     *
     * Returns
     *     direction(s) of reassembly for session
     */
    static char get_reassembly_direction(Flow*);

    /* Get reassembly flush_policy for given session
     *
     * Returns
     *     flush policy for specified direction
     */
    static char get_reassembly_flush_policy(Flow*, char dir);

    /* Get true/false as to whether stream data is in
     * sequence or packets are missing
     *
     * Returns
     *     true/false
     */
    static char is_stream_sequenced(Flow*, char dir);

    /* Get whether there are missing packets before, after or
     * before and after reassembled buffer
     *
     * Returns
     *      SSN_MISSING_BOTH if missing before and after
     *      SSN_MISSING_BEFORE if missing before
     *      SSN_MISSING_AFTER if missing after
     *      SSN_MISSING_NONE if none missing
     */
    static int missing_in_reassembled(Flow*, char dir);

    /* Get true/false as to whether packets were missed on
     * the stream
     *
     * Returns
     *     true/false
     */
    static char missed_packets(Flow*, char dir);

    /* Get the protocol identifier from a stream
     *
     * Returns
     *     integer protocol identifier
     */
    static int16_t get_application_protocol_id(Flow*);

    /* Set the protocol identifier for a stream
     *
     * Returns
     *     integer protocol identifier
     */
    static int16_t set_application_protocol_id(Flow*, int16_t appId);

    /*  Get an independent bit to allow an entity to enable and
     *  disable port session tracking and syn session creation
     *  without affecting the status of set by other entities.
     *  Returns a bitmask (with the bit range 3-15) or 0, if no bits
     *  are available.
     */
    static uint16_t get_preprocessor_status_bit(void);

    // initialize response count and expiration time
    static void init_active_response(Packet*, Flow*);

    /* Get the current flush point
     *
     * Returns
     *  Current flush point for session
     */
    static uint32_t get_flush_point(Flow*, char dir);

    /* Set the next flush point
     */
    static void set_flush_point(Flow*, char dir, uint32_t fpt);

    int set_paf_callback(PAF_Callback);
    PAF_Callback get_paf_callback(unsigned);

    // get any paf user data stored for this session
    static void** get_paf_user_data(Flow*, bool toServer);

    static bool is_paf_active(Flow*, bool toServer);
    static bool activate_paf(Flow*, bool toServer);

    /*  Set flag to force sessions to be created on SYN packets.
     *  This function can only be used with independent bits
     *  acquired from get_preprocessor_status_bit. If this is called
     *  during parsing a preprocessor configuration, make sure to
     *  set the parsing argument to 1.
     */
    static void set_tcp_syn_session_status(SnortConfig* sc, uint16_t status);
    /*  Unset flag that forces sessions to be created on SYN
     *  packets. This function can only be used with independent
     *  bits acquired from get_preprocessor_status_bit. If this is
     *  called during parsing a preprocessor configuration, make
     *  sure to set the parsing argument to 1.
     */
    static void unset_tcp_syn_session_status(SnortConfig* sc, uint16_t status);
    /* Turn off inspection for potential session.
     * Adds session identifiers to a hash table.
     * TCP only.
     *
     * Returns
     *     0 on success
     *     -1 on failure
     */
    int set_application_protocol_id_expected(
        snort_ip_p a1, uint16_t p1, snort_ip_p a2, uint16_t p2, uint8_t proto,
        int16_t appId, FlowData*);

    /** Retrieve application session data based on the lookup tuples for
     *  cases where Snort does not have an active packet that is
     *  relevant.
     *
     * Returns
     *     Application Data reference (pointer)
     */
    static FlowData* get_application_data_from_ip_port(
        snort_ip_p a1, uint16_t p1, snort_ip_p a2, uint16_t p2, char proto,
        uint16_t vlanId, uint32_t mplsId, uint16_t addrSpaceId, unsigned flow_id);

    /*  Get the application data from the session key
     */
    static FlowData* get_application_data_from_key(const FlowKey*, unsigned flow_id);

    // -- extra data methods
    uint32_t reg_xtra_data_cb(LogFunction);
    void reg_xtra_data_log(LogExtraData, void*);
    uint32_t get_xtra_data_map(LogFunction**);

    static void set_extra_data(Flow*, Packet *, uint32_t);
    static void clear_extra_data(Flow*, Packet *, uint32_t);
    void log_extra_data(Flow*, uint32_t mask, uint32_t id, uint32_t sec);

    /** Retrieve stream session pointer based on the lookup tuples for
     *  cases where Snort does not have an active packet that is
     *  relevant.
     *
     * Returns
     *     Stream session pointer
     */
    static Flow* get_session_ptr_from_ip_port(
        snort_ip_p a1, uint16_t p1, snort_ip_p a2, uint16_t p2, char proto,
        uint16_t vlanId, uint32_t mplsId, uint16_t addrSpaceId);

    /* Delete the session if it is in the closed session state.
     */
    void check_session_closed(Packet*);

    /*  Create a session key from the Packet
     */
    static FlowKey* get_session_key(Packet*);

    /*  Populate a session key from the Packet
     */
    static void populate_session_key(Packet*, FlowKey*);

    // register for stateful scanning of in-order payload to determine flush points
    // autoEnable allows PAF regardless of s5 ports config
    static bool register_paf_service(
            SnortConfig*, uint16_t service, bool toServer,
        PAF_Callback, bool autoEnable);

    // register returns a non-zero id for use with set; zero is error
    unsigned register_event_handler(Stream_Callback);
    static bool set_event_handler(Flow*, unsigned id, Stream_Event);

    void call_handler(Packet* p, unsigned id);

    void update_direction(Flow*, char dir, snort_ip_p ip, uint16_t port);

    static void set_application_protocol_id_from_host_entry(
        Flow *lwssn, struct _HostAttributeEntry *host_entry, int direction);

    static uint32_t set_session_flags(Flow*, uint32_t flags);
    static uint32_t get_session_flags(Flow*);

    static bool is_midstream(Flow* flow)
        { return flow->s5_state.session_flags & SSNFLAG_MIDSTREAM; };

    static int get_ignore_direction(Flow*);
    static int set_ignore_direction(Flow*, int ignore_direction);

    // Get the TTL value used at session setup
    // outer=0 to get inner ip ttl for ip in ip; else outer=1
    static uint8_t get_session_ttl(Flow*, char dir, int outer);

    static bool expired_session (Flow*, Packet*);
    static bool ignored_session (Flow*, Packet*);
    static bool blocked_session (Flow*, Packet*);

private:
    static void set_ip_protocol(Flow*);

private:
    uint32_t xtradata_func_count = 0;
    LogFunction xtradata_map[MAX_LOG_FN];
    LogExtraData extra_data_log = NULL;
    void *extra_data_config = NULL;

    Stream_Callback stream_cb[MAX_EVT_CB];
    unsigned stream_cb_idx = 1;

    PAF_Callback s5_cb[MAX_PAF_CB];
    uint8_t s5_cb_idx;
};

/**Port Inspection States. Port can be either ignored,
 * or inspected or session tracked. The values are bitmasks.
 */
typedef enum {
    /**Dont monitor the port. */
    PORT_MONITOR_NONE = 0x00,

    /**Inspect the port. */
    PORT_MONITOR_INSPECT = 0x01,

    /**perform session tracking on the port. */
    PORT_MONITOR_SESSION = 0x02

} PortMonitorStates;

extern Stream stream;

#define PORT_MONITOR_SESSION_BITS   0xFFFE

#endif
