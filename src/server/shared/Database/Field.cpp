////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Field.h"

Field::Field()
{
    data.value = NULL;
    data.type = MYSQL_TYPE_NULL;
    data.length = 0;
    data.raw = false;
}

Field::~Field()
{
    CleanUp();
}

void Field::SetByteValue(void* newValue, enum_field_types newType, uint32 length)
{
    // This value stores raw bytes that have to be explicitly cast later
    data.value = newValue;
    data.length = length;
    data.type = newType;
    data.raw = true;
}

void Field::SetStructuredValue(char* newValue, enum_field_types newType)
{
    if (data.value)
        CleanUp();

    // This value stores somewhat structured data that needs function style casting
    if (newValue)
    {
        size_t size = strlen(newValue);
        data.value = new char [size+1];
        strcpy((char*)data.value, newValue);
        data.length = size;
    }

    data.type = newType;
    data.raw = false;
}
