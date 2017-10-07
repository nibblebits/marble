#ifndef CUSTOMPOINTERS_H

#include <memory>
#include <iostream>
#include <vector>

template <typename T>
class SharedSelfHoldingPointer;

template<typename T>
class SharedSelfHoldingPointer
{
private:
        struct sp_values
        {
            T* value;
            int* count;
            bool* points_to_self;
        };
        struct sp_values value;
        static std::vector<sp_values> allocations;
        
        sp_values get_stored_instance(T* address)
        {
            for (sp_values& v : allocations)
            {
                if (v.value == address)
                    return v;
            }
            throw std::logic_error("Instance not found");
        }
        
public:
        SharedSelfHoldingPointer()
        {
            this->value.value = NULL;
            this->value.points_to_self = NULL;
        }
        
        SharedSelfHoldingPointer(T* value)
        {
        
            if (is_object_stored(value))
            {
                this->value = get_stored_instance(value);
                *this->value.count += 1;
                return;
            }
            
            this->value.count = new int;
            this->value.points_to_self = new bool;
            *this->value.count = 1;
                
            this->value.value = value;
            *this->value.points_to_self = false;
            this->allocations.push_back(this->value);
        }
        
        SharedSelfHoldingPointer(SharedSelfHoldingPointer<T> const &sp)
        {    
            this->value = sp.value;
            *this->value.count+=1;
        }
        

        ~SharedSelfHoldingPointer()
        {
            if (this->value.value == NULL)
            {
                return;
            }
            *this->value.count-=1;       
            if (use_count() == 0 || (*this->value.points_to_self && use_count() == 1))
            {
               delete this->value.count;
               delete this->value.value;
               delete this->value.points_to_self;
               return;
            }     
        }
        
        bool is_object_stored(T* address)
        {
            for (sp_values& v : allocations)
            {
                if (v.value == address)
                    return true;
            }
            
            return false;
        }
        
        const SharedSelfHoldingPointer<T>& operator=(const SharedSelfHoldingPointer<T>&& other)
        {
            this->value = other.value;
            *this->value.count += 1;
            return *this;
        }
        
        void mark_points_to_self()
        {
            *this->value.points_to_self = true;
        }
        
        T& operator*()
        {
            return *this->value.value;
        }
        
        T* operator->()
        {
            return this->value.value;
        }
        
        int use_count()
        {
            return *this->value.count;
        }
};

template <typename T>
typename std::vector<typename SharedSelfHoldingPointer<T>::sp_values>  SharedSelfHoldingPointer<T>::allocations;


#endif
