# --------------------------------------------------------------------
#  通用設定
# --------------------------------------------------------------------
CC      := gcc
CFLAGS  := -O3 -std=c11 -Wall -Wextra
LDFLAGS := -lgmp

# --------------------------------------------------------------------
#  共用來源
# --------------------------------------------------------------------
COMMON_SRCS := \
    ../helper.c \
    ../../src/p256bits/p256bits_inv.c \
    ../../src/p256bits/divstepx19/divstepx19_ref.c \
    ../../src/p256bits/divstepx20/divstepx20_ref.c \
    ../../src/p256bits/extraction_to_uvrs/extraction_to_uvrs_ref.c \
    ../../src/p256bits/final_adjustment/final_adjustment_ref.c \
    ../../src/p256bits/inner_update_fg/inner_update_fg_ref.c \
    ../../src/p256bits/inner_update_uuvvrrss/inner_update_uuvvrrss_ref.c \
    ../../src/p256bits/low60bits/low60bits_ref.c \
    ../../src/p256bits/update_FG/update_FG_neon.c \
    ../../src/p256bits/update_VS/update_VS_mont.c

# --------------------------------------------------------------------
#  個別目標設定
# --------------------------------------------------------------------
single_test_SRCS := single_test.c $(COMMON_SRCS)
random_test_SRCS := random_test.c $(COMMON_SRCS)

BINARIES := single_test random_test

# --------------------------------------------------------------------
#  規則
# --------------------------------------------------------------------
.PHONY: all clean
all: $(BINARIES)

single_test: $(single_test_SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

random_test: $(random_test_SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	$(RM) $(BINARIES)