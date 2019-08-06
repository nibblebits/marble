/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

/*
    Anti Circle Shared Pointer is a shared pointer implementation that can break out of circular references
*/
#ifndef ACSP_H
#define ACSP_H

#include <stdio.h>
#include <iostream>
#include <map>
#include <memory>
#include <exception>

class RC
{
  private:
    int count; // Reference count
    int stuck_count; // The total cycles we are stuck in
  public:
    void markStuck()
    {
        stuck_count += 1;
    }

    void markUnstuck()
    {
        stuck_count -= 1;
    }

    bool isStuck()
    {
        return stuck_count > 0;
    }

    int getStuckCount()
    {
        return stuck_count;
    }

    void AddRef()
    {
        // Increment the reference count
        count++;
    }

    int Release()
    {
        // Decrement the reference count and
        // return the reference count.
        int c = --count;
        if (c == stuck_count)
        {
            count = 0;
            return count;
        }
        return c;
    }

    int getRef()
    {
        return this->count;
    }
};

template <typename T>
class ACSP
{
  private:
    T *pData;                                   // pointer
    std::shared_ptr<RC> reference;                              // Reference count
    std::shared_ptr<std::map<T *, int>> circle; // The referencing circle
  protected:
    void createCircle()
    {
        if (circle != NULL)
            throw std::logic_error("Circle is not NULL");
        circle = std::make_shared<std::map<T *, int>>();
        addToCircle(pData);
    }

    bool hasInCircle(T* pValue)
    {
        return ((*circle).find(pValue) != (*circle).end());
    }

    void addToCircle(T *pValue, int count_to_add = 1, bool exactly=false)
    {
        // Ignore NULL's
        if (pValue == NULL)
            return;

        if (exactly)
        {
            (*circle)[pValue] = count_to_add;
            return;
        }

        int total_count = 0;
        if (hasInCircle(pValue))
        {
            total_count = (*circle)[pValue];
        }
        
        total_count += count_to_add;
        (*circle)[pValue] = total_count;
    }


    void decrementCircleElement(T *pValue)
    {
        (*circle)[pValue] = (*circle)[pValue] - 1;
        if ((*circle)[pValue] <= 0)
        {
            (*circle).erase(pValue);
        }

        if (reference->getRef() == 1 && (*circle).size() > 1)
        {
         // throw std::logic_error("Circular reference problem detected");
        }
    }

  public:
    std::shared_ptr<std::map<T *, int>> getCircle()
    {
        return circle;
    }

    ACSP() : pData(0), reference(0)
    {
        // Create inner circle
        createCircle();
        // Create a new reference
        reference = std::make_shared<RC>();
        // Increment the reference count
        reference->AddRef();
    }

    ACSP(T *pValue) : pData(pValue), reference(0)
    {
        // Create inner circle
        createCircle();

        // Create a new reference
        reference = std::make_shared<RC>();
        // Increment the reference count
        reference->AddRef();
    }

    ACSP(const ACSP<T>&sp) : pData(sp.pData), reference(sp.reference), circle(sp.circle)
    {
        // Copy constructor
        // Copy the data and reference pointer
        // and increment the reference count
        reference->AddRef();
    }

    ~ACSP()
    {
        // Destructor
        // Decrement the reference count
        // if reference become zero delete the data

        int ref_count = reference->Release();
        decrementCircleElement(pData);

        if (ref_count == 0)
        {
            delete pData;
        }

    }

    T *get()
    {
        return pData;
    }

    T &operator*()
    {
        return *pData;
    }

    T *operator->()
    {
        return pData;
    }

    RC* getReference()
    {
        return this->reference.get();
    }

    ACSP<T>&operator=(const ACSP<T>&sp)
    {
        // Assignment operator
        if (this != &sp) // Avoid self assignment
        {
            // Decrement the old reference count
            // if reference become zero delete the old data
            int ref_count = reference->Release();
            decrementCircleElement(pData);
            if (ref_count == 0)
            {
                delete pData;
            }

            // Copy the data and reference pointer
            // and increment the reference count
            pData = sp.pData;
            reference = sp.reference;
            circle = sp.circle;
            reference->AddRef();
        }
        return *this;
    }

    static void CircleMerge(ACSP<T>&dst, ACSP<T>&other)
    {
        bool is_stuck = false;
        if (other.hasInCircle(dst.get()))
        {
            dst.getReference()->markStuck();
            other.getReference()->markStuck();
        }
        
        for (auto const &x : *other.circle)
        {
            dst.addToCircle(x.first, x.second, true);
        }

        other.circle = dst.circle;
    }

    static void CircleRelease(ACSP<T>&dst, ACSP<T>&other)
    {
        if (other.hasInCircle(dst.get()))
        {
            dst.getReference()->markUnstuck();
            other.getReference()->markUnstuck();
        }

        for (auto const &x : *other.circle)
        {
            dst.decrementCircleElement(x.first);
        }
    }
};
#endif
