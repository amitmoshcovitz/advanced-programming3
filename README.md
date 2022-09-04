
<h1 align="center">
  <br>
  <a href="https://github.com/amitmoshcovitz/advanced-programming"><img src="https://img.icons8.com/color/344/c-plus-plus-logo.png" alt="C++" width="200"></a>
  <br>
  advanced-programming
  <br>
</h1>

<h4 align="center">This github repository is for the assignments given in advanced programming course. <a href="https://lemida.biu.ac.il/course/view.php?id=71359" target="_blank">Moodle</a>.</h4>


<p align="center">
  <a href="#team">Team</a> •
  <a href="#key-features">Key Features</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#implementation">Implementation</a>
</p>



## Team
<table>
  <tr>
    <th style="text-align:center"><a href="https://github.com/amitmoshcovitz"><img src="https://avatars.githubusercontent.com/u/79401964?v=4" alt="Amit Moshcovitz" width="140" height="140"></a></th>
    <th style="text-align:center"><a href="https://github.com/YoavNahumus"><img src="https://avatars.githubusercontent.com/u/62788945?v=4" alt="Yoav Nahum" width="140" height="140"></a</th>
  </tr>
  <tr>
    <th>Amit Moshcovitz</td>
    <th>Yoav Nahum</td> 
  </tr>

</table>


## Key Features

* Classify objects.
  - Dynamic value, choose your own classification constance.
  - Lots of distances options (Euclidean, Chebyshev...).
* Works with clients and a main server.

## How To Use

To clone and run this application, you'll need [Git](https://git-scm.com) installed on your computer. From your command line:

```bash
# Clone this repository
$ git clone https://github.com/amitmoshcovitz/advanced-programming3

# Go into the repository
$ cd advanced-programming3

# Compile the server
$ g++ *.cpp server/server.cpp -o server.out -std=c++11

# Compile the client
$ g++ *.cpp client/client.cpp -o client.out -std=c++11

# Run the server
$ ./server.out

# Run the client
$ ./client.out
```

## Implementation
* The server classification logic:
  - A point class that represent a generic point in space.
  - The classification of every point is saved in a map. The key is the reference and the value is a string representation of the classification.
  - Find k closest objects in O(n) time complexity using the quick select algorithm.
  - Iterating the k closest points and deciding the classification of the object based on the largest number of simular points.
* Calculating the Confusion Matrix:
  - A confusion matrix is a matrix that shows the percentage of correct classification.
  - We can calculate the confusion matrix by iterating the classification map and comparing the classification of the object with the real classification.
* Rounding the Percentages in the Confusion Matrix:
  - We can calculate the percentages in the confusion matrix by dividing the number of correct classification by the total number of classification.
  - Rounding to 100%: We calculate the remainder of the division for each percentage and the amount left to get to 100%. We add 1 to the percentages with the highest remainder.
* The server:
  - Creates a string containing all the information from the socket.
  - Runs the algorithms that was in the previous assignment.
  - Sends the results to the server.
* The client:
  - Creates a string containing all the information of the provided file.
  - Sends the string to the server.
  - Writing the result to a file.


## Emailware

This repository is an [emailware](https://en.wiktionary.org/wiki/emailware). Meaning, if you liked using this app or are inspired by the beautiful coding, I'd like you to send me an email at <yoavnahum2005@gmail.com> or <amitmos2005@gmail.com> about anything you'd want to say about this masterpiece. I'd really appreciate it!
