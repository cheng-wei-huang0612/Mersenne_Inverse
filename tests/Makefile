CC     = gcc
CFLAGS = -I../include -O2
LDFLAGS = -lgmp
# 如果需要 GMP，就加上 -lgmp
# LDFLAGS = -lgmp


# 通用的 SRC (可以視需求篩選或增減)

SRC = $(wildcard ../src/p255m19/p255m19_inv.c \
	  ../src/p255m19/divstep_ref.c \
	  ../src/p255m19/extraction_ref.c \
	  ../src/p255m19/inner_update_ref.c \
	  helper.c \)




all: single_test random_test

single_test: single_test.c $(SRC)
	$(CC) $(CFLAGS) single_test.c $(SRC) $(LDFLAGS) -o single_test

random_test: random_test.c $(SRC)
	$(CC) $(CFLAGS) random_test.c $(SRC) $(LDFLAGS) -o random_test


clean:
	rm -f single_test random_test 

