//--------------------------------------------------------------------------
// Copyright (C) 2016-2016 Cisco and/or its affiliates. All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License Version 2 as published
// by the Free Software Foundation.  You may not use, modify or distribute
// this program under any other version of the GNU General Public License.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//--------------------------------------------------------------------------

//dce_tcp.h author Maya Dagon <mdagon@cisco.com>
// based on work by Todd Wease

#ifndef DCE_UDP_H
#define DCE_UDP_H

#include "dce_common.h"
#include "dce_list.h"
#include "protocols/packet.h"
#include "profiler/profiler.h"
#include "framework/counts.h"

#define DCE2_UDP_NAME "dce_udp"
#define DCE2_UDP_HELP "dce over udp inspection"

struct dce2UdpStats
{
    /* The common stats block has to be at the beginning followed
       by the protocol specific stats */

    /*common stats -defined in common.h*/
    PegCount events;
    PegCount sessions_aborted;
    PegCount bad_autodetects;

    /*DCE UDP specific*/
    PegCount udp_sessions;
    PegCount udp_pkts;
    PegCount cl_pkts;
    PegCount cl_request;
    PegCount cl_ack;
    PegCount cl_cancel;
    PegCount cl_cli_fack;
    PegCount cl_ping;
    PegCount cl_response;
    PegCount cl_reject;
    PegCount cl_cancel_ack;
    PegCount cl_srv_fack;
    PegCount cl_fault;
    PegCount cl_nocall;
    PegCount cl_working;
    PegCount cl_other_req;
    PegCount cl_other_resp;
    PegCount cl_fragments;
    PegCount cl_max_frag_size;
    PegCount cl_frag_reassembled;
    PegCount cl_max_seqnum;
};

extern THREAD_LOCAL dce2UdpStats dce2_udp_stats;
extern THREAD_LOCAL ProfileStats dce2_udp_pstat_main;
extern THREAD_LOCAL ProfileStats dce2_udp_pstat_session;
extern THREAD_LOCAL ProfileStats dce2_udp_pstat_new_session;
extern THREAD_LOCAL ProfileStats dce2_udp_pstat_detect;
extern THREAD_LOCAL ProfileStats dce2_udp_pstat_log;
extern THREAD_LOCAL ProfileStats dce2_udp_pstat_cl_acts;
extern THREAD_LOCAL ProfileStats dce2_udp_pstat_cl_frag;
extern THREAD_LOCAL ProfileStats dce2_udp_pstat_cl_reass;

struct DCE2_ClTracker
{
    DCE2_List* act_trackers;  /* List of activity trackers */
};

struct DCE2_UdpSsnData
{
    DCE2_SsnData sd;  // This member must be first
    DCE2_ClTracker cl_tracker;
};

class Dce2UdpFlowData : public FlowData
{
public:
    Dce2UdpFlowData();
    ~Dce2UdpFlowData();

    static void init()
    {
        flow_id = FlowData::get_flow_id();
    }

public:
    static unsigned flow_id;
    DCE2_UdpSsnData dce2_udp_session;
};

#endif

