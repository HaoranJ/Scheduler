Scheduler Implementation in a Discrete Event Simulation
——————————————————————————————————————

How to compile:
>> make

How to run:
>> ./simulator [-v] [-s<schedspec>] inputfile rfile
(-v means verbose, you can see the details of the scheduler;
<schedspec> means the specific scheduler algorithms: F = first-come first-served, 
L = last-come first-served, S = shortest job first, R2 = round-robin with quantum 2,
P5 = priority scheduling with quantum 5)

How to generate all testing results:
>> ./runit.sh <outdir> ./simulator

How to compare the differences:
>> ./diffit.sh refout <outdir>