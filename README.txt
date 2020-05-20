Name: Raunak
ASU ID: 1217240245
Assignment 1

Steps to execute:
A. Compile by using make command.
B. Initialize the driver by using init_rb.sh.
C. Run the rb_tree file.
D. Rmove the driver by using exit_rb.sh.

Number of operations can be changed by changing the variables in main.c file.
By default kprobe works for rb_insert write function. It can be changed by reversing the value in krobe test function in main file.


Output for 10 writes, 5 reads and 5 kprobe:

registration of kprobe was successful!
Thread ID accessing the tree for dev0 = 139927475398400
Thread ID accessing the tree for dev0 = 139927467005696
data entered for dev0 : key =  83, data = 1 
Operations finished for dev2= 1 
Thread ID accessing the tree for dev1 = 139927450220288
---------------------------------------
data entered for dev1 : key =  86, data = 1 
Operations finished for dev1 = 1 
---------------------------------------
Thread ID accessing the tree for dev1 = 139927458612992
data entered for dev0 : key =  77, data = 1 
Operations finished for dev2= 2 
---------------------------------------
data entered for dev1 : key =  15, data = 1 
Operations finished for dev1 = 2 
---------------------------------------
Reading the KPROBE buffer values....
pid = 3061, address = 0xffffffffc0a71060, timestamp (TSC) = 871003723016, Thread ID accessing the tree for dev0 = 139927467005696
data entered for dev0 : key =  93, data = 1 
Operations finished for dev2= 3 
---------------------------------------
Reading the KPROBE buffer values....
pid = 3060, address = 0xffffffffc0a71060, timestamp (TSC) = 881373035680, Thread ID accessing the tree for dev0 = 139927475398400
data entered for dev0 : key =  35, data = 1 
Operations finished for dev2= 4 
---------------------------------------
Reading the KPROBE buffer values....
pid = 3059, address = 0xffffffffc0a71060, timestamp (TSC) = 886556274304, Thread ID accessing the tree for dev1 = 139927458612992
data entered for dev1 : key =  86, data = 1 
Operations finished for dev1 = 3 
---------------------------------------
Reading the KPROBE buffer values....
pid = 3061, address = 0xffffffffc0a71060, timestamp (TSC) = 889149241012, Thread ID accessing the tree for dev0 = 139927467005696
data entered for dev0 : key =  92, data = 1 
Operations finished for dev2= 5 
---------------------------------------
Reading the KPROBE buffer values....
pid = 3060, address = 0xffffffffc0a71060, timestamp (TSC) = 891742662306, Thread ID accessing the tree for dev0 = 139927475398400
IOCTL Id is 2147773185 , cmd :0 
Ioctl set. It returned : 0
Reading Value in Ascending order for dev0 : 35
Operations finished for dev2= 6 
---------------------------------------
Thread ID accessing the tree for dev0 = 139927467005696
IOCTL Id is 2147773185 , cmd :1 
Ioctl set. It returned : 0
Reading Value in Descending order for dev0 : -1
Operations finished for dev2= 7 
---------------------------------------
Thread ID accessing the tree for dev1 = 139927450220288
data entered for dev1 : key =  63, data = 1 
Operations finished for dev1 = 4 
---------------------------------------
Thread ID accessing the tree for dev1 = 139927458612992
data entered for dev1 : key =  26, data = 1 
Operations finished for dev1 = 5 
---------------------------------------
Thread ID accessing the tree for dev0 = 139927467005696
IOCTL Id is 2147773185 , cmd :0 
Ioctl set. It returned : 0
Reading Value in Ascending order for dev0 : 77
Operations finished for dev2= 8 
---------------------------------------
Thread ID accessing the tree for dev0 = 139927475398400
IOCTL Id is 2147773185 , cmd :0 
Ioctl set. It returned : 0
Reading Value in Ascending order for dev0 : 83
Operations finished for dev2= 9 
---------------------------------------
Thread ID accessing the tree for dev0 = 139927467005696
IOCTL Id is 2147773185 , cmd :0 
Ioctl set. It returned : 0
Reading Value in Ascending order for dev0 : 92
Operations finished for dev2= 10 
---------------------------------------
Thread ID accessing the tree for dev1 = 139927458612992
IOCTL Id is 2147773185 , cmd :1 
Ioctl set. It returned : 0
Reading Value in Descending order for dev1 : 86
Operations finished for dev1 = 6 
---------------------------------------
Thread ID accessing the tree for dev1 = 139927450220288
IOCTL Id is 2147773185 , cmd :1 
Ioctl set. It returned : 0
Reading Value in Descending order for dev1 : 63
Operations finished for dev1 = 7 
---------------------------------------
Thread ID accessing the tree for dev1 = 139927458612992
IOCTL Id is 2147773185 , cmd :0 
Ioctl set. It returned : 0
Reading Value in Ascending order for dev1 : 86
Operations finished for dev1 = 8 
---------------------------------------
Thread ID accessing the tree for dev1 = 139927458612992
IOCTL Id is 2147773185 , cmd :1 
Ioctl set. It returned : 0
Reading Value in Descending order for dev1 : 63
Operations finished for dev1 = 9 
---------------------------------------
Thread ID accessing the tree for dev1 = 139927450220288
IOCTL Id is 2147773185 , cmd :0 
Ioctl set. It returned : 0
Reading Value in Ascending order for dev1 : 86
Operations finished for dev1 = 10 
---------------------------------------
Read the tree of size for dev0: 5
ALL THE TREE DATA for dev0: 
35,77,83,92,93,
Read the tree of size for dev1: 4
ALL THE TREE DATA for dev1: 
15,26,63,86,

