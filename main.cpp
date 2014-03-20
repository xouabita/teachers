#ifndef __PROGTEST__

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include <set>

using namespace std;
#endif

#define vChr(c) (( ((c)>=65&&(c)<=90) || ((c)>=97&&(c)<=122) ) ? true : false )

typedef map<string,set<string> > MyMap;
typedef unsigned char uchar;
bool sep1 (uchar c) { return ( c==':' ? true : false); }
bool sep2 (uchar c) { return ( (c==',' || c=='\n')? true : false); }

void writeString (FILE * file, string str) {
  for (int i = 0; i < str.size(); ++i) {
    fwrite(&str[i],1,1,file);
  }
}

bool addProf(MyMap &courses, string course, string prof) {
  if (courses.find(course) == courses.end()) {
    set<string> tmp;
    tmp.insert(prof);
    courses[course] = tmp;
  } else {
    if (courses[course].find(prof) != courses[course].end()) { courses[course].insert(prof); }
    else { return false; }
  }
  return true;
}

bool extract (FILE * file, bool (* fun)(uchar), string &res, uchar &lchr) {
  uchar aChar; int space = true;
  fread(&aChar,1,1,file);
  while (true) {
    if (feof(file)) { return false; }
    if ((*fun)(aChar)) { break; }
    if (vChr(aChar)) {
      if (space) { res += aChar;}
      else { return false; }
    } else if (aChar != ' ') { return false; }
  }
  lchr = aChar;
  return true;
}

bool extract (FILE * file, bool (* fun)(uchar), string &res) {
  uchar c;
  return extract(file,(*fun),res,c);
}

bool skipComm (FILE * file) {
  uchar c;
  while (!feof(file)) {
    fread(&c,1,1,file);
    if (c == '\n') { return true; }
  }
  return false;
}

bool transform (const char * inFile, const char * outFile) {

  FILE * input = fopen(inFile, "rb");
  if (input == NULL) { return false; }
  uchar aChar;
  MyMap courses;

  while (!feof(input)) {

    // Read each line
    fread(&aChar, 1, 1, input);
    if (aChar == '#') {
      if (!skipComm (input)) {return false;} }
    else {
      string prof = "";
      if (vChr(aChar)) { prof += aChar; }
      if (!extract(input,sep1,prof)) { return false; }
      uchar lChar;
      string course = "";
      while (true) {
        if (!extract(input,sep2,course,lChar)) { return false; }
        if (!addProf(courses,course,prof)) { return false; }
        if (lChar == '\n') { break; }
        course = "";
      }
    }
  }

  FILE * output = fopen(outFile, "wb");
  if (output == NULL) { return false; }
  for (MyMap::iterator i = courses.begin(); i != courses.end(); ++i) {
    writeString(output, i->first);
    writeString(output, ": ");
    for (set<string>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
      writeString(output,*j);
      if (*j == *i->second.rbegin()) { writeString(output,"\n"); }
      else { writeString(output,", "); }
    }
  }

  return true;
}

#ifndef __PROGTEST__
int main (int argc, char * argv []) {

  transform (argv[1],argv[2]);

  return 0;
}
#endif
