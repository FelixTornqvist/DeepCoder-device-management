printf "\n===one test 100 times===\n"
for value in {1..5}
do
  python ../test_DH.py xml-json/test4.xml json xml-json/test4.txt
done