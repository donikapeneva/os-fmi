
#!/bin/bash

user=$(whoami);
find / -user $user 2> /dev/null > res;

echo $(wc -l < res);


echo $(find / 2> /dev/null | wc -l);
echo $(find / -user $user 2>/dev/null | wc -l);

exit 0;
