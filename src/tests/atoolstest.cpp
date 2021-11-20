/*****************************************************************************
* Copyright 2015-2020 Alexander Barthel alex@littlenavmap.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "atoolstest.h"

#include "util/average.h"

AtoolsTest::AtoolsTest()
{

}

void AtoolsTest::testAverageTime()
{
  {
    atools::util::MovingAverageTime average(1000);

    qint64 timestamp = 0;
    average.startSamples(timestamp);
    timestamp += 1;

    for(int i = 0; i < 1000; i++)
    {
      average.addSamples(i, 0.f, timestamp);
      timestamp += 1;
      average.addSamples(i, 0.f, timestamp);
      timestamp += 1;
    }

    float avg1, avg2;
    average.getAverages(avg1, avg2);
    QCOMPARE(avg1, 749.25f);
  }

  {
    atools::util::MovingAverageTime average(10000);

    qint64 timestamp = 0;
    average.startSamples(timestamp);

    for(int i = 0; i < 1000; i++)
    {
      timestamp += 100;
      average.addSamples(5.f, 50.f, timestamp);
      timestamp += 400;
      average.addSamples(10.f, 100.f, timestamp);
    }

    float avg1, avg2;
    average.getAverages(avg1, avg2);
    QCOMPARE(avg1, 9.03846f);
    QCOMPARE(avg2, 90.3846f);
  }
  {
    atools::util::MovingAverageTime average(10000);

    qint64 timestamp = 0;
    average.startSamples(timestamp);

    for(int i = 0; i < 1000; i++)
    {
      timestamp += 250;
      average.addSamples(5.f, 50.f, timestamp);
      timestamp += 250;
      average.addSamples(10.f, 100.f, timestamp);
    }

    float avg1, avg2;
    average.getAverages(avg1, avg2);
    QCOMPARE(avg1, 7.56098f);
    QCOMPARE(avg2, 75.6098f);
  }

  {
    atools::util::MovingAverageTime average(10);

    qint64 timestamp = 0;
    average.startSamples(timestamp);
    timestamp += 1;

    for(int i = 0; i < 100; i++)
    {
      average.addSamples(10.f, 100.f, timestamp);
      timestamp += 1;
    }

    float avg1, avg2;
    average.getAverages(avg1, avg2);
    QCOMPARE(avg1, 10.f);
    QCOMPARE(avg2, 100.f);
  }

  return;
}

void AtoolsTest::testAverage()
{
  {
    atools::util::MovingAverage average(500);
    average.addSample(5.f);
    average.addSample(5.f);
    average.addSample(5.f);
    average.addSample(5.f);
    average.addSample(5.f);

    QCOMPARE(average.getAverage(), 5.f);
  }

  {
    atools::util::MovingAverage average(6);
    average.addSample(5.f);
    average.addSample(10.f);
    average.addSample(5.f);
    average.addSample(10.f);
    average.addSample(5.f);
    average.addSample(10.f);

    QCOMPARE(average.getAverage(), 7.5f);
  }

  {
    atools::util::MovingAverage average(4);
    average.addSample(5.f);
    average.addSample(10.f);
    average.addSample(5.f);
    average.addSample(10.f);
    average.addSample(5.f);
    average.addSample(10.f);

    QCOMPARE(average.getAverage(), 7.5f);
  }

  {
    atools::util::MovingAverage average(2);
    average.addSample(5.f);
    average.addSample(10.f);
    average.addSample(5.f);
    average.addSample(10.f);
    average.addSample(5.f);
    average.addSample(10.f);

    QCOMPARE(average.getAverage(), 7.5f);
  }

}