TARGET ?=download
INCDIRS	 := ./
SRCDIRS  := ./
INCLUDE  := $(patsubst %,-I %, $(INCDIRS))				  
CFILES   := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CFILENDIR    := $(notdir $(CFILES))
COBJS    := $(patsubst %, %, $(CFILENDIR:.c=.o))
OBJS     := $(COBJS)
VPATH    := $(SRCDIRS)

$(TARGET) : $(OBJS)
	gcc  -o $(TARGET) $^ 
$(COBJS) : %.o : %.c
	gcc -c -O2  $(INCLUDE) -o $@ $<	

clean:
	rm -rf $(TARGET) $(OBJS)
