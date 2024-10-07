#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
SUCCESS_leak=0
FAIL_leak=0
COUNTER_leak=0
DIFF=""

s21_command=(
    "./../s21_grep"
)

sys_command=(
    "grep"
)

tests=(
"FLAGS test_files/test1.txt"
"FLAGS test_files/test2.txt"
"FLAGS test_files/test3.txt"
"FLAGS test_files/test1.txt test_files/test2.txt"
"FLAGS test_files/test1.txt test_files/test3.txt"
"FLAGS test_files/test2.txt test_files/test1.txt"
"FLAGS test_files/test2.txt test_files/test3.txt"
"FLAGS test_files/test3.txt test_files/test1.txt"
"FLAGS test_files/test3.txt test_files/test2.txt"
"FLAGS test_files/test1.txt test_files/test2.txt test_files/test3.txt"
)

templates=(
    "9"
    "r"
    "R"
)

flags=(
    "i"
    "v"
    "c"
    "l"
    "n"
    "h"
    "s"
    "o"
)

manual=(
"9 test_files/test1.txt"
"r test_files/test2.txt"
"d test_files/test3.txt"
"R test_files/test1.txt test_files/test2.txt"
"r test_files/test1.txt test_files/test3.txt"
""[0-9]" test_files/test2.txt test_files/test1.txt"
"r test_files/test2.txt test_files/test3.txt"
"x test_files/test3.txt test_files/test1.txt"
"d test_files/test3.txt test_files/test2.txt"
"9i test_files/test1.txt test_files/test2.txt test_files/test3.txt"
"-e 9 test_files/test1.txt"
"-i d test_files/test2.txt"
"-v r test_files/test1.txt"
"-n 9 test_files/test1.txt"
"-h 9 test_files/test1.txt"
"-s 9 test_files/test1.txt nofile.txt"
"-o 9 test_files/test1.txt"
"-l 9 test_files/test1.txt"
"-c 9 test_files/test1.txt"
)

run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER ok $param"
    else
        let "FAIL++"
        echo "$COUNTER bad $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}
run_leak_test() {
    param=$(echo $@ | sed "s/FLAGS/$var/")
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -q --log-file="${s21_command[@]}".log ./"${s21_command[@]}" $param > /dev/null
    leak=$(grep -ic -A10000 "LEAK SUMMARY:" "${s21_command[@]}".log || true)
    leak2=$(grep -ic -A10000 "ERROR SUMMARY: [^0]" "${s21_command[@]}".log || true)
    let "COUNTER_leak++"
    if [ "$leak" -eq "0" ] && [ "$leak2" -eq "0" ]
    then
        let "SUCCESS_leak++"
        echo "$COUNTER_leak - Success $param"
    else
        let "FAIL_leak++"
        echo "$COUNTER_leak - Fail $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

printf "\nman tests\n\n"
for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done

printf "\ntests with 1 flag\n\n"
for var2 in "${templates[@]}"
do
  for var1 in "${flags[@]}"
  do
    for i in "${tests[@]}"
    do
        var="-$var1 $var2"
        run_test "$i"
    done
  done
done
printf "\ntests with 2 flag\n\n"
for var3 in "${templates[@]}"
do
  for var1 in "${flags[@]}"
  do
      for var2 in "${flags[@]}"
      do
          if [ $var1 != $var2 ]
          then
              for i in "${tests[@]}"
              do
                  var="-$var1 -$var2 $var3"
                  run_test "$i"
              done
          fi
      done
  done
done
printf "\ntests with 3 flag\n\n"
for var4 in "${templates[@]}"
do
    for var1 in "${flags[@]}"
    do
        for var2 in "${flags[@]}"
        do
            for var3 in "${flags[@]}"
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 $var4"
                        run_test "$i"
                    done
                fi
            done
        done
    done
done
printf "\nman_leak tests\n\n"
for i in "${manual[@]}"
do
    var="-"
    run_leak_test "$i"
done

printf "\nleak tests with 1 flag\n\n"
for var2 in "${templates[@]}"
do
  for var1 in "${flags[@]}"
  do
    for i in "${tests[@]}"
    do
        var="-$var1 $var2"
        run_leak_test "$i"
    done
  done
done
# printf "\ntests with 4 flag\n\n"
# for var1 in "${flags[@]}"
# do
#     for var2 in "${flags[@]}"
#     do
#         for var3 in "${flags[@]}"
#         do
#             for var4 in "${flags[@]}"
#             do
#                 if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
#                 && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
#                 && [ $var2 != $var4 ] && [ $var3 != $var4 ]
#                 then
#                     for i in "${tests[@]}"
#                     do
#                         var="-$var1 -$var2 -$var3 -$var4"
#                         run_test "$i"
#                     done
#                 fi
#             done
#         done
#     done
# done
printf "\n"
echo "failed: $FAIL"
echo "succes: $SUCCESS"
echo "all: $COUNTER"
printf "\n"
printf "\n"
echo "failed leaks: $FAIL_leak"
echo "succes leaks: $SUCCESS_leak"
echo "all leaks: $COUNTER_leak"
printf "\n"
