CC := gcc

EXE := main


BUILD_DIR := ./build

SRC_DIRS := ./src


SRCS := $(shell find $(SRC_DIRS) -name '*.c')

OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find include -type d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := $(INC_FLAGS) -MMD -MP -g



$(BUILD_DIR)/$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) -g

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)

