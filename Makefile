CXXFLAG = -O3 -static-libstdc++ -I ./ -std=c++11 -lm -Wall -fopenmp -D EIGEN_NO_DEBUG
TARGET = bprmf

all: $(TARGET)

bprmf: bprmf.o data.o model.o predict.o
	g++ $(CXXFLAG) -o $@ $^

test_data: test_data.o data.o 
	g++ $(CXXFLAG) -o $@ $^

%.o: %.cpp %.h
	g++ $(CXXFLAG) -o $@ -c $<
%.o: %.cpp
	g++ $(CXXFLAG) -o $@ -c $<

.PHONY: clean
clean:
	$(RM) $(TARGET) *.o *~

test:
	./bprmf -m 1 --train data/real_divide_20130101_510000/train.txt --valid data/real_divide_20130101_510000/valid.txt --rank_matrix me.rank
