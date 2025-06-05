CC     = gcc
CFLAGS = -I../include -O2
LDFLAGS = -lgmp
# 如果需要 GMP，就加上 -lgmp
# LDFLAGS = -lgmp


# 通用的 SRC (可以視需求篩選或增減)

SRC = $(wildcard ../src/p255m19/p255m19_inv.c \
	  ../src/p255m19/initialization_ref.c \
	  ../src/p255m19/low60bits_ref.c \
	  ../src/p255m19/divstepx19_ref.c \
	  ../src/p255m19/divstepx20_ref.c \
	  ../src/p255m19/extraction_to_uvrs_ref.c \
	  ../src/p255m19/extraction_to_uuvvrrss_ref.c \
	  ../src/p255m19/inner_update_uuvvrrss_ref.c \
	  ../src/p255m19/inner_update_fg_with_uuvvrrss_ref.c \
	  ../src/p255m19/inner_update_fg_ref.c \
	  ../src/p255m19/update_FG_ref.c \
	  ../src/p255m19/update_VS_ref.c \
	  ../src/p255m19/final_adjustment_ref.c \
	  helper.c \)




all: single_test random_test

single_test: single_test.c $(SRC)
	$(CC) $(CFLAGS) single_test.c $(SRC) $(LDFLAGS) -o single_test

random_test: random_test.c $(SRC)
	$(CC) $(CFLAGS) random_test.c $(SRC) $(LDFLAGS) -o random_test


clean:
	rm -f single_test random_test 

