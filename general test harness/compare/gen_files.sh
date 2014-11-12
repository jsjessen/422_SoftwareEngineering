#! /bin/bash

make

CMD_FILE="./cmds.sh"

BLOCKS="1440"

DISKS[0]="my_disk"
DISKS[1]="kc_disk"
DISKS[2]="linux_disk"

for disk in "${DISKS[@]}"
do
    rm -f $disk
    touch $disk
    mkfs.ext2 -F $disk $BLOCKS
done

PROGS[1]="my_ext2fs"
PROGS[2]="kc_ext2fs"

for (( i = 0 ; i < ${#PROGS[@]} ; i++ )) do
    cat $CMD_FILE | '${PROGS[$i]} ${DISKS[$i]}' > ${PROGS[$i]}.out
done

mount -o loop ${DISKS[2]} /mnt
cd /mnt
$CMD_FILE ${DISKS[2]} > ${DISKS[2]}.out 
umount /mnt
