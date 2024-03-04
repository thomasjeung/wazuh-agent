/*
 * Copyright (C) 2015, Wazuh Inc.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <time.h>

#include "../../logcollector/journal_log.h"

#include "../wrappers/common.h"
#include "../wrappers/externals/pcre2/pcre2_wrappers.h"

/* setup/teardown */

static int group_setup(void ** state) {
    test_mode = 1;
    w_test_pcre2_wrappers(false);
    return 0;

}

static int group_teardown(void ** state) {
    test_mode = 0;
    w_test_pcre2_wrappers(true);
    return 0;

}



void test_w_journald_poc(void ** state) {



    // Init
    w_journal_context_t* ctx;
    assert_int_equal(0, w_journal_context_create(&ctx));

    // Create filterA
    w_journal_filter_t * filterA = NULL;
    assert_int_equal(0, w_journal_filter_add_condition(&filterA, "PRIORITY", "5|6", false));
    assert_int_equal(0, w_journal_filter_add_condition(&filterA, "_SYSTEMD_UNIT", "^systemd-journald.service", false));

    // Create filterB
    w_journal_filter_t * filterB = NULL;
    assert_int_equal(0, w_journal_filter_add_condition(&filterB, "_COMM", "^cron", false));

    // Create filterC
    w_journal_filter_t * filterC = NULL;
    assert_int_equal(0, w_journal_filter_add_condition(&filterC, "_COMM", "^sshd", false));
    assert_int_equal(0, w_journal_filter_add_condition(&filterC, "_EXE", "^/usr/sbin/sshd", false));
    assert_int_equal(0, w_journal_filter_add_condition(&filterC, "SYSLOG_IDENTIFIER", "^sshd", false));


    // Seek
    int result = w_journal_context_seek_timestamp(ctx, 1708954788027700);
    // Fail if result < 0
    if(0 > result) {
        assert_true(0);
    }

    int count = 0;
    do
    {

        result = w_journal_context_next_newest(ctx);

        if (result < 0)
        {
            fprintf(stderr, "Failed to get next entry: %s\n", strerror(-result));
            return;
        }
        else if (result == 0)
        {
            //fprintf(stderr, "No new entries\n");
            sleep(1);
            // break;
            continue;
        }

        // Filter
        if (w_journal_filter_apply(ctx, filterA) > 0 || w_journal_filter_apply(ctx, filterB) > 0 ||
            w_journal_filter_apply(ctx, filterC) > 0 || true)
        {
            // Dump, print and free entry
            //w_journal_entry_t* entry = w_journal_entry_dump(ctx, W_JOURNAL_ENTRY_DUMP_TYPE_JSON);
            w_journal_entry_t* entry = w_journal_entry_dump(ctx, W_JOURNAL_ENTRY_DUMP_TYPE_SYSLOG);

            char* entry_str = w_journal_entry_to_string(entry);
            assert_non_null(entry_str);
            printf("%s\n--------%d-------------\n", entry_str, ++count);
            free(entry_str);

            w_journal_entry_free(entry);
        }

    } while (1);

    w_journal_context_free(ctx);

}


int main(void) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_w_journald_poc),
    };

    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
