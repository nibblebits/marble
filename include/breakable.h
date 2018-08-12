#ifndef BREAKABLE_H
#define BREAKABLE_H

#include "statics.h"
/**
*
* Nodes that are breakable should extend this class.
* A node is breakable if it allows supports the "continue" and "break" keywords.
* These are typically used in loops.
*/
class Breakable
{
 public:
     Breakable();
     virtual ~Breakable();
     /**
     * Invoke to break out of this entity.
     * 
     * \param type Requires a BREAK_TYPE to be provided. This BREAK_TYPE will specify the type of break this is such as a break of continue or a break of break.
     */
     void breakNow(BREAK_TYPE type);
     /**
     * Returns true if breakNow has been invoked with a BREAK_TYPE other than BREAK_TYPE_NO_BREAK
     */
     bool isBroken();
     
     /**
     * Returns the BREAK_TYPE of the current break.
     */
     BREAK_TYPE getBreakType();

     /**
      * Releases this break setting it back to BREAK_TYPE_NO_BREAK
      */
     void releaseBreak();
 protected:
     /**
     * This pure method is invoked upon breaking with the "breakNow" method.
     * When releasing breaks this method is also called and BREAK_TYPE_NO_BREAK is passed
     */
     virtual void didBreak(BREAK_TYPE type) = 0;
 private:
     BREAK_TYPE break_type;
};

#endif
