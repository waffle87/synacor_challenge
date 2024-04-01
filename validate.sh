#!/bin/sh

if [ "$#" -eq 0 ]; then
  printf "usage: %s <codes.txt>\n" $0
  exit 1
fi

n=0
master="6fcd818224b42f563e10b91b4f2a5ae8"
sums=(
    "76ec2408e8fe3f1753c25db51efd8eb3"
    "0e6aa7be1f68d930926d72b3741a145c"
    "7997a3b2941eab92c1c0345d5747b420"
    "186f842951c0dcfe8838af1e7222b7d4"
    "2bf84e54b95ce97aefd9fc920451fc45"
    "e09640936b3ef532b7b8e83ce8f125f4"
    "4873cf6b76f62ac7d5a53605b2535a0c"
    "d0c54d4ed7f943280ce3e19532dbb1a6"
)

while read i; do
    curr=$(echo -n "$i" | md5sum $master - 2>/dev/null)
    curr="${curr%?}"
    printf "code %s:\nproduce:\t%s\nexpect:\t\t%s\n" $i $curr ${sums[$n]}
    ((n++))
done < "$1"
