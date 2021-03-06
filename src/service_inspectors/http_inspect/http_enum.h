//--------------------------------------------------------------------------
// Copyright (C) 2014-2016 Cisco and/or its affiliates. All rights reserved.
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
// http_enum.h author Tom Peters <thopeter@cisco.com>

#ifndef HTTP_ENUM_H
#define HTTP_ENUM_H

#include <stdint.h>

namespace HttpEnums
{
static const int MAX_OCTETS = 65535;
static const int DATA_BLOCK_SIZE = 16384;
static const int FINAL_BLOCK_SIZE = 24576;
static const int GZIP_BLOCK_SIZE = 2048;
static const int FINAL_GZIP_BLOCK_SIZE = 2304; // compromise value, too big causes gzip overruns
                                               // too small leaves too many little end sections
static const uint32_t HTTP_GID = 119;
static const int GZIP_WINDOW_BITS = 31;
static const int DEFLATE_WINDOW_BITS = 15;
static const int MAX_FIELD_NAME_LENGTH = 100;

// This can grow into a bitmap for the get_buf() form parameter
static const uint64_t FORM_REQUEST = 0x1;

// Field status codes for when no valid value is present in length or integer value. Positive
// values are actual length or field value.
enum StatusCode { STAT_NO_SOURCE=-16, STAT_NOT_CONFIGURED=-15, STAT_NOT_COMPUTE=-14,
    STAT_PROBLEMATIC=-12, STAT_NOT_PRESENT=-11, STAT_EMPTY_STRING=0, STAT_OTHER=1 };

// Message originator--client or server
enum SourceId { SRC__NOT_COMPUTE=-14, SRC_CLIENT=0, SRC_SERVER=1 };

// Type of message section
enum SectionType { SEC_DISCARD = -19, SEC_ABORT = -18, SEC__NOT_COMPUTE=-14, SEC__NOT_PRESENT=-11,
    SEC_REQUEST = 2, SEC_STATUS, SEC_HEADER, SEC_BODY_CL, SEC_BODY_CHUNK, SEC_TRAILER,
    SEC_BODY_OLD };

// Message buffers available to clients
// This enum must remain synchronized with HttpApi::classic_buffer_names[]
enum HTTP_BUFFER { HTTP_BUFFER_CLIENT_BODY = 1, HTTP_BUFFER_COOKIE, HTTP_BUFFER_HEADER,
    HTTP_BUFFER_METHOD, HTTP_BUFFER_RAW_COOKIE, HTTP_BUFFER_RAW_HEADER, HTTP_BUFFER_RAW_URI,
    HTTP_BUFFER_STAT_CODE, HTTP_BUFFER_STAT_MSG, HTTP_BUFFER_URI, HTTP_BUFFER_VERSION,
    HTTP_BUFFER_TRAILER, HTTP_BUFFER_RAW_TRAILER, HTTP_BUFFER_RAW_REQUEST,
    HTTP_BUFFER_RAW_STATUS, HTTP_BUFFER_MAX };

// Peg counts
// This enum must remain synchronized with HttpModule::peg_names[] in http_tables.cc
enum PEG_COUNT { PEG_FLOW = 0, PEG_SCAN, PEG_REASSEMBLE, PEG_INSPECT, PEG_REQUEST, PEG_RESPONSE,
    PEG_GET, PEG_HEAD, PEG_POST, PEG_PUT, PEG_DELETE, PEG_CONNECT, PEG_OPTIONS, PEG_TRACE,
    PEG_OTHER_METHOD, PEG_REQUEST_BODY, PEG_CHUNKED, PEG_URI_NORM, PEG_URI_PATH, PEG_URI_CODING,
    PEG_COUNT_MAX };

// Result of scanning by splitter
enum ScanResult { SCAN_NOTFOUND, SCAN_FOUND, SCAN_FOUND_PIECE, SCAN_DISCARD, SCAN_DISCARD_PIECE,
    SCAN_ABORT, SCAN_END };

// State machine for chunk parsing
enum ChunkState { CHUNK_ZEROS, CHUNK_NUMBER, CHUNK_WHITESPACE, CHUNK_OPTIONS, CHUNK_HCRLF,
    CHUNK_DATA, CHUNK_DCRLF1, CHUNK_DCRLF2, CHUNK_BAD };

// List of possible HTTP versions.
enum VersionId { VERS__NO_SOURCE=-16, VERS__NOT_COMPUTE=-14, VERS__PROBLEMATIC=-12,
    VERS__NOT_PRESENT=-11, VERS__OTHER=1, VERS_1_0, VERS_1_1, VERS_2_0, VERS_0_9 };

// Every request method we have ever heard of
enum MethodId { METH__NO_SOURCE=-16, METH__NOT_COMPUTE=-14, METH__PROBLEMATIC=-12,
    METH__NOT_PRESENT=-11, METH__OTHER=1, METH_OPTIONS, METH_GET, METH_HEAD, METH_POST, METH_PUT,
    METH_DELETE, METH_TRACE, METH_CONNECT, METH_PROPFIND, METH_PROPPATCH, METH_MKCOL, METH_COPY,
    METH_MOVE, METH_LOCK, METH_UNLOCK, METH_VERSION_CONTROL, METH_REPORT, METH_CHECKOUT,
    METH_CHECKIN, METH_UNCHECKOUT, METH_MKWORKSPACE, METH_UPDATE, METH_LABEL, METH_MERGE,
    METH_BASELINE_CONTROL, METH_MKACTIVITY, METH_ORDERPATCH, METH_ACL, METH_PATCH, METH_SEARCH,
    METH_BCOPY, METH_BDELETE, METH_BMOVE, METH_BPROPFIND, METH_BPROPPATCH, METH_NOTIFY, METH_POLL,
    METH_SUBSCRIBE, METH_UNSUBSCRIBE, METH_X_MS_ENUMATTS, METH_BIND, METH_LINK, METH_MKCALENDAR,
    METH_MKREDIRECTREF, METH_REBIND, METH_UNBIND, METH_UNLINK, METH_UPDATEREDIRECTREF };

// URI formats
enum UriType { URI__NOT_COMPUTE=-14, URI__PROBLEMATIC=-12, URI_ASTERISK = 2, URI_AUTHORITY,
    URI_ABSPATH, URI_ABSOLUTE };

// Body compression tpyes
enum CompressId { CMP_NONE=2, CMP_GZIP, CMP_DEFLATE };

// Message section in which an IPS option provides the buffer
enum InspectSection { IS_NONE, IS_DETECTION, IS_BODY, IS_TRAILER };

// Part of the URI to be provided
enum UriComponent { UC_SCHEME = 1, UC_HOST, UC_PORT, UC_PATH, UC_QUERY, UC_FRAGMENT };

// Every header we have ever heard of
enum HeaderId { HEAD__NOT_COMPUTE=-14, HEAD__PROBLEMATIC=-12, HEAD__NOT_PRESENT=-11, HEAD__OTHER=1,
    HEAD_CACHE_CONTROL, HEAD_CONNECTION, HEAD_DATE, HEAD_PRAGMA, HEAD_TRAILER, HEAD_COOKIE,
    HEAD_SET_COOKIE, HEAD_TRANSFER_ENCODING, HEAD_UPGRADE, HEAD_VIA, HEAD_WARNING, HEAD_ACCEPT,
    HEAD_ACCEPT_CHARSET, HEAD_ACCEPT_ENCODING, HEAD_ACCEPT_LANGUAGE, HEAD_AUTHORIZATION,
    HEAD_EXPECT, HEAD_FROM, HEAD_HOST, HEAD_IF_MATCH, HEAD_IF_MODIFIED_SINCE, HEAD_IF_NONE_MATCH,
    HEAD_IF_RANGE, HEAD_IF_UNMODIFIED_SINCE, HEAD_MAX_FORWARDS, HEAD_PROXY_AUTHORIZATION,
    HEAD_RANGE, HEAD_REFERER, HEAD_TE, HEAD_USER_AGENT, HEAD_ACCEPT_RANGES, HEAD_AGE, HEAD_ETAG,
    HEAD_LOCATION, HEAD_PROXY_AUTHENTICATE, HEAD_RETRY_AFTER, HEAD_SERVER, HEAD_VARY,
    HEAD_WWW_AUTHENTICATE, HEAD_ALLOW, HEAD_CONTENT_ENCODING, HEAD_CONTENT_LANGUAGE,
    HEAD_CONTENT_LENGTH, HEAD_CONTENT_LOCATION, HEAD_CONTENT_MD5, HEAD_CONTENT_RANGE,
    HEAD_CONTENT_TYPE, HEAD_EXPIRES, HEAD_LAST_MODIFIED, HEAD_X_FORWARDED_FOR, HEAD_TRUE_CLIENT_IP,
    HEAD__MAX_VALUE };

// All the infractions we might find while parsing and analyzing a message
enum Infraction
{
    INF_BARE_BYTE = 0,
    INF_HEAD_TOO_LONG,
    INF_BAD_REQ_LINE,
    INF_BAD_STAT_LINE,
    INF_TOO_MANY_HEADERS,
    INF_BAD_HEADER,
    INF_BAD_STAT_CODE,
    INF_UNKNOWN_VERSION,
    INF_BAD_VERSION,
    INF_ZERO_NINE_NOT_FIRST,
    INF_URI_IIS_UNICODE,
    INF_BAD_HEADER_DATA,
    INF_PIPELINE_OVERFLOW,
    INF_BAD_CHUNK_SIZE,
    INF_BAD_PHRASE,
    INF_BAD_URI,
    INF_ZERO_NINE_REQ,
    INF_ZERO_NINE_CONTINUE,
    INF_URI_PERCENT_UTF8_3B,
    INF_URI_PERCENT_UNRESERVED,
    INF_URI_PERCENT_UTF8_2B,
    INF_NOT_USED_1,
    INF_URI_PERCENT_OTHER,
    INF_URI_BAD_CHAR,
    INF_URI_8BIT_CHAR,
    INF_URI_MULTISLASH,
    INF_URI_BACKSLASH,
    INF_URI_SLASH_DOT,
    INF_URI_SLASH_DOT_DOT,
    INF_URI_ROOT_TRAV,
    INF_TOO_MUCH_LEADING_WS,
    INF_WS_BETWEEN_MSGS,
    INF_ENDLESS_HEADER,
    INF_LF_WITHOUT_CR,
    INF_NOT_HTTP,
    INF_NO_URI,
    INF_REQUEST_WS,
    INF_REQUEST_TAB,
    INF_STATUS_WS,
    INF_STATUS_TAB,
    INF_URI_SPACE,
    INF_TOO_LONG_HEADER,
    INF_LONE_CR,
    INF_CHUNK_ZEROS,
    INF_CHUNK_OPTIONS,
    INF_CHUNK_BAD_CHAR,
    INF_CHUNK_TOO_LARGE,
    INF_CHUNK_BARE_LF,
    INF_CHUNK_LONE_CR,
    INF_CHUNK_NO_LENGTH,
    INF_CHUNK_BAD_END,
    INF_PARTIAL_START,
    INF_CHUNK_WHITESPACE,
    INF_HEAD_NAME_WHITESPACE,
    INF_GZIP_OVERRUN,
    INF_GZIP_FAILURE,
    INF_GZIP_EARLY_END,
    INF_URI_NEED_NORM_PATH,
    INF_URI_NEED_NORM_HOST,
    INF_URI_NEED_NORM_QUERY,
    INF_URI_NEED_NORM_FRAGMENT,
    INF_URI_U_ENCODE,
    INF_URI_UNKNOWN_PERCENT,
    INF_URI_DOUBLE_DECODE,
    INF_MULTIPLE_CONTLEN,
    INF_BOTH_CL_AND_TE,
    INF_BAD_CODE_BODY_HEADER,
    INF_FINAL_NOT_CHUNKED,
    INF_CHUNKED_BEFORE_END,
    INF_OVERSIZE_DIR,
    INF_POST_WO_BODY,
    INF_UTF_NORM_FAIL,
    INF_UTF7,
    INF_UNSUPPORTED_ENCODING,
    INF_UNKNOWN_ENCODING,
    INF_STACKED_ENCODINGS,
    INF__MAX_VALUE
};

// Types of character for URI scanning
enum CharAction { CHAR_NORMAL=2, CHAR_PERCENT, CHAR_PATH, CHAR_EIGHTBIT, CHAR_SUBSTIT };

// Transfer codings
enum Transcoding { TRANSCODE__OTHER=1, TRANSCODE_CHUNKED, TRANSCODE_GZIP, TRANSCODE_DEFLATE,
    TRANSCODE_COMPRESS, TRANSCODE_X_GZIP, TRANSCODE_X_COMPRESS, TRANSCODE_IDENTITY };

// Content codings
enum Contentcoding { CONTENTCODE__OTHER=1, CONTENTCODE_GZIP, CONTENTCODE_DEFLATE,
    CONTENTCODE_COMPRESS, CONTENTCODE_EXI, CONTENTCODE_PACK200_GZIP, CONTENTCODE_X_GZIP,
    CONTENTCODE_X_COMPRESS, CONTENTCODE_IDENTITY };

enum EventSid
{
    EVENT_ASCII = 1,
    EVENT_DOUBLE_DECODE,
    EVENT_U_ENCODE,
    EVENT_BARE_BYTE,
    EVENT_OBSOLETE_1,
    EVENT_UTF_8,
    EVENT_IIS_UNICODE,
    EVENT_MULTI_SLASH,
    EVENT_IIS_BACKSLASH,
    EVENT_SELF_DIR_TRAV,
    EVENT_DIR_TRAV,
    EVENT_APACHE_WS,
    EVENT_IIS_DELIMITER,
    EVENT_NON_RFC_CHAR,
    EVENT_OVERSIZE_DIR,
    EVENT_LARGE_CHUNK,
    EVENT_PROXY_USE,
    EVENT_WEBROOT_DIR,
    EVENT_LONG_HDR,
    EVENT_MAX_HEADERS,
    EVENT_MULTIPLE_CONTLEN,
    EVENT_CHUNK_SIZE_MISMATCH,
    EVENT_INVALID_TRUEIP,
    EVENT_MULTIPLE_HOST_HDRS,
    EVENT_LONG_HOSTNAME,
    EVENT_EXCEEDS_SPACES,
    EVENT_CONSECUTIVE_SMALL_CHUNKS,
    EVENT_UNBOUNDED_POST,
    EVENT_MULTIPLE_TRUEIP_IN_SESSION,
    EVENT_BOTH_TRUEIP_XFF_HDRS,
    EVENT_UNKNOWN_METHOD,
    EVENT_SIMPLE_REQUEST,
    EVENT_UNESCAPED_SPACE_URI,
    EVENT_PIPELINE_MAX,
    EVENT_ANOM_SERVER,
    EVENT_INVALID_STATCODE,
    EVENT_NO_CONTLEN,
    EVENT_UTF_NORM_FAIL,
    EVENT_UTF7,
    EVENT_DECOMPR_FAILED,
    EVENT_CONSECUTIVE_SMALL_CHUNKS_S,
    EVENT_MSG_SIZE_EXCEPTION,
    EVENT_JS_OBFUSCATION_EXCD,
    EVENT_JS_EXCESS_WS,
    EVENT_MIXED_ENCODINGS,
    EVENT_SWF_ZLIB_FAILURE,
    EVENT_SWF_LZMA_FAILURE,
    EVENT_PDF_DEFL_FAILURE,
    EVENT_PDF_UNSUP_COMP_TYPE,
    EVENT_PDF_CASC_COMP,
    EVENT_PDF_PARSE_FAILURE,
    EVENT_LOSS_OF_SYNC,
    EVENT_CHUNK_ZEROS,
    EVENT_WS_BETWEEN_MSGS,
    EVENT_URI_MISSING,
    EVENT_CTRL_IN_REASON,
    EVENT_IMPROPER_WS,
    EVENT_BAD_VERS,
    EVENT_UNKNOWN_VERS,
    EVENT_BAD_HEADER,
    EVENT_CHUNK_OPTIONS,
    EVENT_URI_BAD_FORMAT,
    EVENT_UNKNOWN_PERCENT,
    EVENT_BROKEN_CHUNK,
    EVENT_CHUNK_WHITESPACE,
    EVENT_HEAD_NAME_WHITESPACE,
    EVENT_GZIP_OVERRUN,
    EVENT_GZIP_FAILURE,
    EVENT_ZERO_NINE_CONTINUE,
    EVENT_ZERO_NINE_NOT_FIRST,
    EVENT_BOTH_CL_AND_TE,
    EVENT_BAD_CODE_BODY_HEADER,
    EVENT_FINAL_NOT_CHUNKED,
    EVENT_CHUNKED_BEFORE_END,
    EVENT_MISFORMATTED_HTTP,
    EVENT_UNSUPPORTED_ENCODING,
    EVENT_UNKNOWN_ENCODING,
    EVENT_STACKED_ENCODINGS,
    EVENT__MAX_VALUE
};

extern const int8_t as_hex[256];
extern const bool token_char[256];
extern const bool is_sp_tab[256];
} // end namespace HttpEnums

#endif

