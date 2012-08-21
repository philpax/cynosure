OBJS=arithmetic.o control_flow.o interrupt.o main.o memory.o opcodes.o string.o

.PHONY: all clean

all: machinecode

machinecode: $(OBJS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $^

%.o: %.cpp *.hpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm *.o
	rm -f machinecode