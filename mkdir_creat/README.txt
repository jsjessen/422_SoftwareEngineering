The generalized test harness we worked on for our meeting Wed Nov 12 has been moved.
It can be accessed using the softlink "sidetrack" in this directory.
------------------------------------------------------------------------------------

Previously, when we looked at the functions involved in creat and mkdir
We saw the functions we were going to rely on for the unit tests
The situation made it appear that the unit tests relied on circular logic and were generally error prone.
The general test harness was worked on, but not completed, in an effort to circumvent the problem.

We will now be focusing on using get_inode() and get_block() to make unit tests for mkdir and creat.
As Evan pointed out, the domain of these functions is the disk, so even though they are used by creat and mkdir 
they can still be used to independently verify creation of files

It would be prudent to check that these functions work correctly

When a file is added with creat, 

If the disk contains files A B C and has an inode bitmap of 1111 1111 1100 0000
create or mkdir should ialloc the first free inode
so the first added file should have ino 11
2nd: 12
3rd: 13
4th: 14
...

There should be no matching ino
adding an invalid file and checking the inode we would predict it having if it were valid, the bit should be 0 and the inode bad


creat file1
umount
imap bit 1 flipped
no 0 bit before it
super/gd free_inodes 1 less
valid inode number
search can find file1 (has entry in parent dir) (shows up with ls)
can print correct information with ino->INODE*

subsequent files added > ino
if it was fresh disk, no removes, ino increasing by 1 for each

mkdir same stuff, but should also have a data block
can cd into
proper link count for a new dir (2 because . and parent)
pwd inside it works


How much should we check?
Which checks are valid?

Give a disk in a known state
List of commands to be executed by program
Check against expected values

Compare with KC's output?
Don't compare ouputs directly, can use to help come up with expected values for a test.

Might as well do rm/rmdir at the same time
make files, remove file, make something invalid, remove something invalid
do invarious orders


