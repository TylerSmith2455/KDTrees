# CS315 Makefile for Program 3: k-d trees
# Feel free to modify as needed.
# Original author: Raphael Finkel 1/2020

CFLAGS = -Wall -g
CPPFLAGS = -Wall -g
GOAL = kd

DIMENSION = 3
NUMPOINTS = 50
SEED = 42
MODULUS = 100
NUMPROBES = 1


# compile and run the student program, placing the result in tmpRun.out
run: $(GOAL) randGen.pl
	./randGen.pl $(SEED) $(MODULUS) | \
		./$(GOAL) $(DIMENSION) $(NUMPOINTS) $(NUMPROBES) > tmpRun.out
	less tmpRun.out

# compile and run a known working program, placing the result in tmpWorking.out
runWorking: workingKD randGen.pl
	./randGen.pl $(SEED) $(MODULUS) | \
		./workingKD $(DIMENSION) $(NUMPOINTS) $(NUMPROBES) > tmpWorking.out
	less tmpWorking.out

# get the randGen.pl program
randGen.pl:
	wget http://www.cs.uky.edu/~raphael/courses/CS315/utils/randGen.pl
	chmod +x randGen.pl

# get the working program
workingKD:
	wget http://www.cs.uky.edu/~raphael/courses/CS315/prog3/workingKD
	chmod +x workingKD

# Modify the following recipe to zip exactly what you want to include.
zipAll: 
	zip toSubmit.zip $(GOAL).* Makefile README tmpRun.out
	
.PHONY: clean
clean:
	rm -f randGen.pl $(GOAL) workingKD tmp* toSubmit.zip
