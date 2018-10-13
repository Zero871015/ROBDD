# ROBDD

## Digital Logic Design Project A1 – ROBDD Generation
## Due: 23:55, Oct. 15, 2018
>> Binary Decision Diagram (BDD) is a directed acyclic graph, which is useful to
represent the Boolean function. Each BDD node is associate with one Boolean
variable. There are two outgoing edges in each internal node: the then-edge and the
else-edge. The then-edge indicates that the Boolean variable is “1” and the else-edge
indicates that the Boolean variable is “0”. The true/false decision is made via the
evaluation of variables from the root node to the leaf node. Given a fixed BDD
variable ordering, we are able to generate a unique ordered BDD (OBDD) for every
distinct Boolean function. An OBDD is said to be a reduced OBDD (ROBDD) when
the following properties are satisfied: (1) all isomorphic nodes (i.e., Boolean variable,
then-edge, and else-edge are the same) are merged into one single unique node and (2)
The redundant node (i.e., both of its then-edge and else-edge point to the same node)
is removed. In this project, you are going to process a Boolean function in PLA
format and output your ROBDD in DOT format.

---

## How to use it in Liunx?
* Download the file in your computer(OS:Linux).
* Type "g++ main.cpp -std=c++11" in your CMD.
  * Remember you need to use cd change your directory where main.cpp is.
  * If it succeed, you will see a.out added in the folder.
* And type "./a.out input.pla" to execute the program.
* If it works, you will see the result on CMD, and there are files named obdd.dot and robdd.dot in the folder.

## Contact
* If there is any bug or question, you can use email contact me:
  * zero871015@gmail.com

Thank you.</br>
-Zero871015 10/08/2018
