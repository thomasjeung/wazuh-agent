/*
 * Copyright (C) 2015-2019, Wazuh Inc.
 * January 17, 2018.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

 #ifndef _DEBUG_MESSAGES__H
 #define _DEBUG_MESSAGES__H

/* vulnerability-detector messages*/
#define VU_DOWNLOAD           "(5450): Downloading %s database..."
#define VU_OVA_UPDATED        "(5451): %s OVAL has been updated correctly."
#define VU_START_SCAN         "(5452): Starting vulnerability scanning."
#define VU_END_SCAN           "(5453): Vulnerability scanning finished."
#define VU_START_AG_AN        "(5454): Analyzing agent %s vulnerabilities..."
#define VU_DETECTED_VUL       "(5455): Vulnerability %s detected in agent %s affecting: %s."
#define VU_NOT_VULN           "(5456): The '%s' package from agent %s is not vulnerable to %s. Condition: package version (%s) %s %s."
#define VU_UPDATE_DATE        "(5457): %s OVAL is in its latest version. Update date: %s"
#define VU_START_REFRESH_DB   "(5458): Refreshing %s databases..."
#define VU_STOP_REFRESH_DB    "(5459): Refresh of %s database finished."
#define VU_DB_TIMESTAMP_OVAL  "(5460): %s OVAL has not been downloaded before, so the update continues."
#define VU_STARTING_UPDATE    "(5461): Starting %s database update..."
#define VU_AGENT_SOFTWARE_REQ "(5462): Getting agent %s software..."
#define VU_AGENT_UNSOPPORTED  "(5463): Agent %s has an unsupported Wazuh version."
#define VU_UNS_OS_VERSION     "(5464): %s version not supported (agent %s)."
#define VU_AGENT_PENDING      "(5465): Agent %s operating system could not be obtained because it has never been connected on. It will be omitted..."
#define VU_UNS_OS             "(5466): Unsupported OS. Agent %s will be omitted..."
#define VU_PACK_VER_VULN      "(5467): The '%s' package from agent %s is vulnerable to %s. Condition: package version (%s) %s %s."
#define VU_PACK_VULN          "(5468): The '%s' package is vulnerable to %s."
#define VU_UPDATE_PRE         "(5469): Preparse step."
#define VU_UPDATE_PAR         "(5470): Parse step."
#define VU_UPDATE_VU_CO       "(5471): Inserting vulnerability conditions..."
#define VU_UPDATE_VU_INFO     "(5472): Inserting vulnerability info..."
#define VU_UPDATE_VU          "(5473): Inserting vulnerabilities..."
#define VU_AGENT_INFO_ERROR   "(5474): Agent %s operating system could not be obtained. Maybe it is never connected. It will be omitted..."
#define VU_NO_SOFTWARE        "(5475): No changes have been found with respect to the last package inventory for agent %s."
#define VU_AG_NO_TARGET       "(5476): The analysis can not be launched because there are no target agents."
#define VU_AG_DISC            "(5477): The vulnerabilities of the agent %s will not be checked because it is disconnected."
#define VU_LOCAL_FETCH        "(5478): Fetching OVAL from '%s'..."
#define VU_OPERATION_NOT_REC  "(5479): Operation '%s' not recognized."
#define VU_DOUBLE_NOT_VULN    "(5480): The '%s' package from agent %s is not vulnerable to %s. Condition: package version (%s) %s %s and %s %s."
#define VU_DOUBLE_VULN        "(5481): The '%s' package from agent %s is vulnerable to %s. Condition: package version (%s) %s %s and %s %s."
#define VU_UNEXP_VALUE        "(5482): Unexpected %s attribute."
#define VU_SOL_PATCHES        "(5483): Solving patches..."
#define VU_INS_TEST_SEC       "(5484): Inserting test section..."
#define VU_SYS_CHECKED        "(5485): The last package inventory for the agent %s (ID: %s) has already been checked. The vulnerability search is omitted."
#define VU_AGENT_START        "(5486): Starting vulnerability assessment for agent %s."
#define VU_AGENT_FINISH       "(5487): Finished vulnerability assessment for agent %s."
#define VU_AG_NEVER_CON       "(5488): Agent '%s' never connected."
#define VU_API_REQ_RETRY      "(5490): There was no valid response to '%s'. Retrying in %d seconds..."
#define VU_UNEXP_JSON_KEY     "(5492): Unexpected JSON key: '%s'."
#define VU_ENDING_UPDATE      "(5494): The update of the feeds ended successfully."
#define VU_DOWNLOAD_FAIL      "(5495): The download can not be completed. Retrying in %d seconds."


/* File integrity monitoring debug messages */
#define FIM_DIFF_SKIPPED                    "(6200): Diff execution skipped for containing insecure characters."
#define FIM_SCHED_BATCH                     "(6201): Setting SCHED_BATCH returned: '%d'"
#define FIM_LOCAL_DIFF_DELETE               "(6202): Deleting backup '%s'. Not monitored anymore."
#define FIM_FILE_IGNORE_RESTRICT            "(6203): Ignoring file '%s' for a restriction..."
#define FIM_IGNORE_ENTRY                    "(6204): Ignoring '%s' '%s' by '%s'"
#define FIM_IGNORE_SREGEX                   "(6205): Ignoring '%s' '%s' by sregex '%s'"
#define FIM_TAG_ADDED                       "(6206): Adding tag '%s' to directory '%s'."
#define FIM_WINREGISTRY_START               "(6207): Starting os_winreg_check"
#define FIM_READING_REGISTRY                "(6208): Attempt to read: '%s%s'"
#define FIM_CLIENT_CONFIGURATION            "(6209): Reading Client Configuration [%s]"
#define FIM_LOCALDIFF_DELETE                "(6210): Deleting '%s' from local hash table."
#define FIM_CANNOT_ACCESS_FILE              "(6211): Cannot access '%s': it was removed during scan."
#define FIM_SCANNING_FILE                   "(6212): File '%s'"
#define FIM_SIMBOLIC_LINK_DISABLE           "(6213): Follow symbolic links disabled."
#define FIM_CHECK_LINK_REALPATH             "(6214): Error checking realpath() of link '%s'"
#define FIM_HASH_ADD_FAIL                   "(6215): Not enough memory to add inode to db: '%s' (%s) "
#define FIM_HASH_UPDATE                     "(6216): Updating path '%s' in inode hash table: '%s' (%s) "
#define FIM_SCANNING_IRREGFILE              "(6217): IRREG File: '%s'"
#define FIM_MAX_RECURSION_LEVEL             "(6218): Max level of recursion reached. File '%s' out of bounds."
#define FIM_SYMBOLIC_LINK_DISCARDED         "(6219): Discarding symbolic link '%s' is already added in the configuration.",
#define FIM_SYMBOLIC_LINK_ADD               "(6220): Directory added to FIM configuration by link '%s'"
#define FIM_FILE_MSG_DELETE                 "(6221): Sending delete msg for file: '%s'"
#define FIM_FREQUENCY_DIRECTORY             "(6222): Directory loaded from syscheck db: '%s'"
#define FIM_STAT_FAILED                     "(6223): Stat() function failed on: '%s'. File may have been deleted"
#define FIM_SKIP_NFS                        "(6224): FIM skip_nfs=%d, '%s'::is_nfs=%d"

#define FIM_REALTIME_HASH_DUP               "(6225): Entry '%s' already exists in the RT hash table."
#define FIM_REALTIME_MONITORING             "(6226): The '%s' directory starts to be monitored in real-time mode"
#define FIM_REALTIME_NEWPATH                "(6227): Scanning new file '%s' with options for directory '%s'."
#define FIM_REALTIME_NEWDIRECTORY           "(6228): Directory added for real time monitoring: '%s'."
#define FIM_REALTIME_DISCARD_EVENT          "(6229): Inotify event with same checksum for file: '%s'. Ignoring it."

#define FIM_WHODATA_HANDLE_UPDATE           "(6230): The handler ('%s') will be updated."
#define FIM_WHODATA_NEWDIRECTORY            "(6231): Monitoring with Audit: '%s'."
#define FIM_WHODATA_SCAN                    "(6232): The '%s' directory has been scanned after detecting event of new files."
#define FIM_WHODATA_SCAN_ABORTED            "(6233): Scanning of the '%s' directory is aborted because something has gone wrong."
#define FIM_WHODATA_CHECKTHREAD             "(6234): Checking thread set to '%d' seconds."
#define FIM_WHODATA_DEL_ADD                 "(6235): '%s' has been deleted and added after the last scan."
#define FIM_WHODATA_FULLQUEUE               "(6236): Real-time Whodata events queue for Windows is full. Removing the first '%d'..."
#define FIM_WHODATA_EVENT_DELETED           "(6237): '%d' events have been deleted from the whodata list."
#define FIM_WHODATA_EVENTQUEUE_VALUES       "(6238): Whodata event queue values for Windows -> max_size:'%d' | max_remove:'%d' | alert_threshold:'%d'.",
#define FIM_WHODATA_IGNORE                  "(6239): The file '%s' has been marked as ignored. It will be discarded."
#define FIM_WHODATA_NOT_ACTIVE              "(6240): '%s' is discarded because its monitoring is not activated."
#define FIM_WHODATA_CANCELED                "(6241): The monitoring of '%s' in whodata mode has been canceled. Added to the ignore list."
#define FIM_WHODATA_DIRECTORY_SCANNED       "(6242): The '%s' directory has been scanned. It does not need to do it again."
#define FIM_WHODATA_NEW_FILES               "(6243): New files have been detected in the '%s' directory after the last scan."
#define FIM_WHODATA_DIRECTORY_DISCARDED     "(6244): The '%s' directory has been discarded because it is not being monitored in whodata mode."
#define FIM_WHODATA_CHECK_NEW_FILES         "(6245): New files have been detected in the '%s' directory and will be scanned."
#define FIM_WHODATA_NO_NEW_FILES            "(6246): The '%s' directory has not been scanned because no new files have been detected. Mask: '%x'"
#define FIM_WHODATA_INVALID_UID             "(6247): Invalid identifier for user '%s'"
#define FIM_AUDIT_EVENT                     "(6248): audit_event: uid=%s, auid=%s, euid=%s, gid=%s, pid=%i, ppid=%i, inode=%s, path=%s, pname=%s",
#define FIM_AUDIT_EVENT1                    "(6249): audit_event_1/2: uid=%s, auid=%s, euid=%s, gid=%s, pid=%i, ppid=%i, inode=%s, path=%s, pname=%s",
#define FIM_AUDIT_EVENT2                    "(6250): audit_event_2/2: uid=%s, auid=%s, euid=%s, gid=%s, pid=%i, ppid=%i, inode=%s, path=%s, pname=%s",
#define FIM_AUDIT_DELETE_RULE               "(6251): Deleting Audit rules..."
#define FIM_AUDIT_MATCH_KEY                 "(6252): Match audit_key: '%s'"
#define FIM_HEALTHCHECK_CREATE              "(6253): Whodata health-check: Detected file creation event (%s)."
#define FIM_HEALTHCHECK_DELETE              "(6254): Whodata health-check: Detected file deletion event (%s)."
#define FIM_HEALTHCHECK_UNRECOGNIZED_EVENT  "(6255): Whodata health-check: Unrecognized event (%s)"
#define FIM_HEALTHCHECK_THREAD_ATIVE        "(6256): Whodata health-check: Reading thread active."
#define FIM_HEALTHCHECK_THREAD_FINISED      "(6257): Whodata health-check: Reading thread finished."
#define FIM_HEALTHCHECK_WAIT_CREATE         "(6258): Whodata health-check: Waiting creation event..."
#define FIM_HEALTHCHECK_CREATE_RECEIVE      "(6259): Whodata health-check: Creation event received."
#define FIM_HEALTHCHECK_WAIT_DELETE         "(6260): Whodata health-check: Waiting deletion event..."
#define FIM_HEALTHCHECK_DELETE_RECEIVE      "(6261): Whodata health-check: Deletion event received."
#define FIM_HEALTHCHECK_SUCCESS             "(6262): Whodata health-check: Success."

#define FIM_SACL_CHECK_CONFIGURE            "(6263): It is necessary to configure the SACL of '%s'."
#define FIM_SCAL_NOCONFIGURE                "(6264): It is not necessary to configure the SACL of '%s'."
#define FIM_SACL_RESTORED                   "(6265): The SACL of '%s' has been restored correctly."
#define FIM_SACL_CONFIGURE                  "(6266): The SACL of '%s' will be configured."
#define FIM_SACL_NOT_FOUND                  "(6267): No SACL found on target. A new one will be created."
#define FIM_ELEVATE_PRIVILEGE               "(6268): The '%s' privilege has been added."
#define FIM_REDUCE_PRIVILEGE                "(6269): The '%s' privilege has been removed."

#define FIM_AUDIT_NEWRULE                   "(6270): Added audit rule for monitoring directory: '%s'."
#define FIM_AUDIT_RULEDUP                   "(6271): Audit rule for monitoring directory '%s' already added."
#define FIM_AUDIT_NORUNNING                 "(6272): Auditd is not running."
#define FIM_AUDIT_NOCONF                    "(6273): Cannot apply Audit config."
#define FIM_AUDIT_NORULES                   "(6274): No rules added. Audit events reader thread will not start."
#define FIM_AUDIT_RELOADING_RULES           "(6275): Reloading Audit rules..."
#define FIM_AUDIT_RELOADED_RULES            "(6276): Audit rules reloaded: %i"
#define FIM_AUDIT_THREAD_STOPED             "(6277): Audit thread finished."
#define FIM_AUDIT_HEALTHCHECK_RULE          "(6278): Couldn't add audit health check rule."
#define FIM_AUDIT_HEALTHCHECK_START         "(6279): Whodata health-check: Starting..."
#define FIM_AUDIT_HEALTHCHECK_FILE          "(6280): Couldn't create audit health check file."

#define FIM_SYSCOM_ARGUMENTS                "(6281): SYSCOM getconfig needs arguments."
#define FIM_SYSCOM_UNRECOGNIZED_COMMAND     "(6282): SYSCOM Unrecognized command '%s'."
#define FIM_SYSCOM_FAIL_GETCONFIG           "(6283): At SYSCOM getconfig: Could not get '%s' section"
#define FIM_SYSCOM_REQUEST_READY            "(6284): Local requests thread ready"
#define FIM_SYSCOM_EMPTY_MESSAGE            "(6285): Empty message from local client."
#define FIM_SYSCOM_THREAD_FINISED           "(6286): Local server thread finished."

#define FIM_HEALTHCHECK_CHECK_RULE          "(6287): Couldn't delete audit health check rule.

#endif
