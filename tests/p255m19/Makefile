###############################################################################
#  tests/Makefile
#
#  - 固定包含頭檔路徑： -I../src/p255m19
#  - USE_GMP=1|0     ： 預設 1（連 -lgmp）
#  - 64 種組合：
#      d19<ver>-d20<ver>-ifg<ver>-uvrs<ver>-uuvvrrss<ver>-low60<ver>
#      (ver = ref | 1)
###############################################################################

# ───────────── 0. 工具與通用旗標 ─────────────
CC          ?= gcc
SRCROOT      := ../src/p255m19
BASE_CFLAGS  := -O3 -I$(SRCROOT)

USE_GMP      ?= 1        # make USE_GMP=0 取消 -lgmp
CFLAGS      += $(BASE_CFLAGS)
ifeq ($(USE_GMP),1)
  LDFLAGS   += -lgmp
endif

# ───────────── 1. 源碼分類 ──────────────────
COMMON_SRC   := $(SRCROOT)/p255m19_inv.c helper.c
OBJ_COMMON   := $(COMMON_SRC:.c=.o)

# 1.1 只有 ref 實作、所有組合共用
STABLE_SRC := $(filter-out $(SRCROOT)/divstepx19/% $(SRCROOT)/divstepx20/% \
                           $(SRCROOT)/inner_update_fg/% \
                           $(SRCROOT)/inner_update_uuvvrrss/% \
                           $(SRCROOT)/extraction_to_uvrs/% \
                           $(SRCROOT)/low60bits/%,                      \
                 $(wildcard $(SRCROOT)/*/*_ref.c))
OBJ_STABLE := $(STABLE_SRC:.c=.o)

# 1.2 六個可切換模組及其版本
DIV19_VARIANTS    := ref 1
DIV20_VARIANTS    := ref 1
IFG_VARIANTS      := ref 1                 # inner_update_fg
UVRS_VARIANTS     := ref 1                 # extraction_to_uvrs
UUVVRRSS_VARIANTS := ref 1                 # inner_update_uuvvrrss
LOW60_VARIANTS    := ref 1                 # low60bits

# helper：$(call pick,<dir>,<ver>) → <root>/<dir>/<dir>_<ver>.o
pick = $(SRCROOT)/$1/$1_$2.o

# ───────────── 2. 產生組合與 OBJ_IMPL_<combo> ─────────
COMBOS :=
$(foreach v19,$(DIV19_VARIANTS),$(foreach v20,$(DIV20_VARIANTS),          \
  $(foreach vfg,$(IFG_VARIANTS),$(foreach vrs,$(UVRS_VARIANTS),           \
    $(foreach vuu,$(UUVVRRSS_VARIANTS),$(foreach v60,$(LOW60_VARIANTS),   \
      $(eval combo := d19$(v19)-d20$(v20)-ifg$(vfg)-uvrs$(vrs)-uuvvrrss$(vuu)-low60$(v60)) \
      $(eval COMBOS += $(combo))                                          \
      $(eval OBJ_IMPL_$(combo) :=                                         \
            $(call pick,divstepx19,$(v19))                                \
            $(call pick,divstepx20,$(v20))                                \
            $(call pick,inner_update_fg,$(vfg))                           \
            $(call pick,extraction_to_uvrs,$(vrs))                        \
            $(call pick,inner_update_uuvvrrss,$(vuu))                    \
            $(call pick,low60bits,$(v60)))                                \
))))))

# ───────────── 3. 物件檔快速目標 ───────────────
.PHONY: objects
objects: $(OBJ_COMMON) $(OBJ_STABLE)                                      \
         $(patsubst %.S,%.o,$(wildcard $(SRCROOT)/divstepx19/*.S))        \
         $(patsubst %.S,%.o,$(wildcard $(SRCROOT)/divstepx20/*.S))        \
         $(patsubst %.S,%.o,$(wildcard $(SRCROOT)/inner_update_fg/*.S))   \
         $(patsubst %.S,%.o,$(wildcard $(SRCROOT)/inner_update_uuvvrrss/*.S)) \
         $(patsubst %.S,%.o,$(wildcard $(SRCROOT)/extraction_to_uvrs/*.S)) \
         $(patsubst %.S,%.o,$(wildcard $(SRCROOT)/low60bits/*.S))         \
         $(patsubst %.c,%.o,$(wildcard $(SRCROOT)/divstepx19/*_ref.c))    \
         $(patsubst %.c,%.o,$(wildcard $(SRCROOT)/divstepx20/*_ref.c))    \
         $(patsubst %.c,%.o,$(wildcard $(SRCROOT)/inner_update_fg/*_ref.c)) \
         $(patsubst %.c,%.o,$(wildcard $(SRCROOT)/inner_update_uuvvrrss/*_ref.c)) \
         $(patsubst %.c,%.o,$(wildcard $(SRCROOT)/extraction_to_uvrs/*_ref.c)) \
         $(patsubst %.c,%.o,$(wildcard $(SRCROOT)/low60bits/*_ref.c))

# 基本編譯規則
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

# ───────────── 4. 自動組合 random_test-<combo> ───────
.SECONDEXPANSION:

define make-random-rule
random_test-$(1): random_test.o $(OBJ_COMMON) $(OBJ_STABLE) $$(OBJ_IMPL_$(1))
	$$(CC) $$(CFLAGS) $$^ $$(LDFLAGS) -o $$@
	@echo "===== Running $$@ ====="
	@./$$@
endef
$(foreach c,$(COMBOS),$(eval $(call make-random-rule,$(c))))

# ───────────── 5. 批量 / 預設目標 ─────────────
.PHONY: all_runs
all_runs: $(addprefix random_test-,$(COMBOS))

all: objects all_runs

# ───────────── 6. clean ──────────────────────
.PHONY: clean
clean:
	@echo "Removing object files & binaries…"
	@find . $(SRCROOT) -name '*.o' -type f -delete
	@rm -f random_test-* single_test-* random_test-run single_test-run

###############################################################################
# 7. 手動選配入口 (run / run_single)
###############################################################################
DIV19     ?= ref
DIV20     ?= ref
IFG       ?= ref
UVRS      ?= ref
UUVVRRSS  ?= ref
LOW60     ?= ref

check-ver = $(if $(filter $(1),ref 1),,$(error [$$(strip $(1))] 只能是 ref 或 1))
$(call check-ver,$(DIV19))
$(call check-ver,$(DIV20))
$(call check-ver,$(IFG))
$(call check-ver,$(UVRS))
$(call check-ver,$(UUVVRRSS))
$(call check-ver,$(LOW60))

OBJ_IMPL_RUN := \
    $(call pick,divstepx19,$(DIV19))               \
    $(call pick,divstepx20,$(DIV20))               \
    $(call pick,inner_update_fg,$(IFG))            \
    $(call pick,extraction_to_uvrs,$(UVRS))        \
    $(call pick,inner_update_uuvvrrss,$(UUVVRRSS)) \
    $(call pick,low60bits,$(LOW60))

random_test-run: random_test.o $(OBJ_COMMON) $(OBJ_STABLE) $(OBJ_IMPL_RUN)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "===== Running $@ (DIV19=$(DIV19) DIV20=$(DIV20) IFG=$(IFG) UVRS=$(UVRS) UUVVRRSS=$(UUVVRRSS) LOW60=$(LOW60)) ====="
	@./$@

single_test-run: single_test.o $(OBJ_COMMON) $(OBJ_STABLE) $(OBJ_IMPL_RUN)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "===== Running $@ (DIV19=$(DIV19) DIV20=$(DIV20) IFG=$(IFG) UVRS=$(UVRS) UUVVRRSS=$(UUVVRRSS) LOW60=$(LOW60)) ====="
	@./$@

.PHONY: run run_single run_rand
run_rand:   random_test-run
run_single: single_test-run
run:        run_rand