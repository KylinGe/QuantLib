
/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email ferdinando@ametrano.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file flatforward.hpp
    \brief flat forward rate term structure
*/

// $Id$

#ifndef quantlib_flat_forward_curve_h
#define quantlib_flat_forward_curve_h

#include <ql/termstructure.hpp>

namespace QuantLib {

    namespace TermStructures {

        class FlatForward : public TermStructure,
                            public Patterns::Observer {
          public:
            // constructors
            FlatForward(const Date& todaysDate,
                        const Date& referenceDate,
                        Rate forward,
                        const DayCounter& dayCounter);
            FlatForward(const Date& todaysDate,
                        const Date& referenceDate,
                        const RelinkableHandle<MarketElement>& forward,
                        const DayCounter& dayCounter);
            // inspectors
            DayCounter dayCounter() const;
            Date todaysDate() const {return todaysDate_; }
            Date referenceDate() const;
            Date maxDate() const;
            // Observer interface
            void update();
          protected:
            Rate zeroYieldImpl(Time, bool extrapolate = false) const;
            DiscountFactor discountImpl(Time,
                bool extrapolate = false) const;
            Rate forwardImpl(Time, bool extrapolate = false) const;
          private:
            Date todaysDate_, referenceDate_;
            DayCounter dayCounter_;
            RelinkableHandle<MarketElement> forward_;
        };

        // inline definitions

        inline FlatForward::FlatForward(const Date& todaysDate,
            const Date& referenceDate,
            Rate forward, const DayCounter& dayCounter)
        : todaysDate_(todaysDate), referenceDate_(referenceDate),
          dayCounter_(dayCounter) {
            forward_.linkTo(
                Handle<MarketElement>(new SimpleMarketElement(forward)));
        }

        inline FlatForward::FlatForward(const Date& todaysDate,
            const Date& referenceDate,
            const RelinkableHandle<MarketElement>& forward,
            const DayCounter& dayCounter)
        : todaysDate_(todaysDate), referenceDate_(referenceDate),
          dayCounter_(dayCounter), forward_(forward) {
            registerWith(forward_);
        }

        inline DayCounter FlatForward::dayCounter() const {
            return dayCounter_;
        }

        inline Date FlatForward::referenceDate() const {
            return referenceDate_;
        }

        inline Date FlatForward::maxDate() const {
            return Date::maxDate();
        }

        inline void FlatForward::update() {
            notifyObservers();
        }

        inline Rate FlatForward::zeroYieldImpl(Time t, bool) const {
            // no forward limit on time
            QL_REQUIRE(t >= 0.0,
                "FlatForward::zeroYieldImpl : "
                "zero yield undefined for time (" +
                DoubleFormatter::toString(t) + ")");
            return forward_->value();
        }

        inline DiscountFactor FlatForward::discountImpl(Time t, bool) const {
            // no forward limit on time
            QL_REQUIRE(t >= 0.0,
                "FlatForward::discountImpl : "
                "discount undefined for time (" +
                DoubleFormatter::toString(t) + ")");
            return DiscountFactor(QL_EXP(-forward_->value()*t));
        }

        inline Rate FlatForward::forwardImpl(Time t, bool) const {
            // no forward limit on time
            QL_REQUIRE(t >= 0.0,
                "FlatForward::forwardImpl : "
                "forward undefined for time (" +
                DoubleFormatter::toString(t) + ")");
            return forward_->value();
        }

    }

}

#endif
