Scheduler Implementation in a Discrete Event Simulation
——————————————————————————————————————

How to compile:
>> make

How to run:
>> ./simulator [-v] [-s<schedspec>] inputfile rfile

How to generate all testing results:
>> ./runit.sh <outdir> ./simulator

How to compare the differences:
>> ./diffit.sh refout <outdir>