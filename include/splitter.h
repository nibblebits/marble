#ifndef SPLITTER_H
#define SPLITTER_H

struct marble_code
{
	int start;
	int end;
	char* data;
}

class Splitter
{
  public:
		  Splitter();
		  virtual ~Splitter();
		  void setData(char* data);
          struct marble_code split();
  private:
		  int getPositionOfNextMarbleTag(int position);
		  char* data;
		  int length;
		  struct marble_code* previous;
}
#endif
