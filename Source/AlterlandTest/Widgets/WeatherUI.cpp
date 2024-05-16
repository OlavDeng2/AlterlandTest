// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherUI.h"

void UWeatherUI::SetCorrectTimeString(int Hours, int Minutes, FString& TimeString)
{
    if (Hours < 10)
    {
        TimeString.Append((FString("0") + FString::FromInt(Hours)));
    }
    else
    {
        TimeString.Append(FString::FromInt(Hours));
    }
    TimeString.Append(FString(":"));

    if (Minutes < 10)
    {
        TimeString.Append((FString("0") + FString::FromInt(Minutes)));

    }
    else
    {
        TimeString.Append(FString::FromInt(Minutes));

    }
}

void UWeatherUI::SetCorrectTimezoneString(int TimeZone, FString& TimeZoneString)
{
    if (TimeZone > 0)
    {
        TimeZoneString.Append("+");
    }
    TimeZoneString.Append(FString::FromInt(TimeZone / 3600));//3600 because that is what converst minutes to hours

    //Only add minutes if it is a timezone that isnt a whole hour
    if (TimeZone % 3600)
    {
        TimeZoneString.Append(FString(":"));
        int minutes = ((TimeZone % 3600) / 60);
        if (minutes < 10)
        {
            TimeZoneString.Append(FString("0"));
        }
        TimeZoneString.Append(FString::FromInt(minutes));//Get the remainder from hours, then convert the reminder to minutes
    }


}
