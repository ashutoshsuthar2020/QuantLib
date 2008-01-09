/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2007 Giorgio Facchinetti

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file optionletstripper.hpp
*/

#ifndef quantlib_strippedoptionlet_hpp
#define quantlib_strippedoptionlet_hpp

#include <ql/termstructures/volatility/optionlet/strippedoptionletbase.hpp>
#include <ql/indexes/iborindex.hpp>
#include <ql/quote.hpp>

namespace QuantLib {

    /*! Helper class to wrap in a StrippedOptionletBase object a matrix of
        exogenously calculated optionlet (i.e. caplet/floorlet) volatilities
        (a.k.a. forward-forward volatilities).
    */
    class StrippedOptionlet : public StrippedOptionletBase {
      public:
        StrippedOptionlet(const Date& referenceDate,
                          Natural settlementDays,
                          const boost::shared_ptr<IborIndex>& iborIndex,
                          const std::vector<Period>& optionletTenors,
                          const std::vector<Rate>& strikes,
                          const std::vector<std::vector<Handle<Quote> > >&,
                          const Calendar& calendar,
                          BusinessDayConvention bdc = Following,
                          const DayCounter& dc = Actual365Fixed());

        //! \name StrippedOptionletBase interface
        //@{
        const std::vector<Rate>& optionletStrikes(Size i) const;
        const std::vector<Volatility>& optionletVolatilities(Size i) const;

        const std::vector<Date>& optionletFixingDates() const;
        const std::vector<Time>& optionletFixingTimes() const;

        const std::vector<Rate>& atmOptionletRates() const;

        DayCounter dayCounter() const;
        Calendar calendar() const;
        Natural settlementDays() const;
        BusinessDayConvention businessDayConvention() const;
        const Date& referenceDate() const;
        //@}

        const std::vector<Period>& optionletTenors() const;

      private:
        void checkInputs() const;
        void registerWithMarketData();
        void performCalculations() const;

        Date referenceDate_;
        Calendar calendar_;
        Natural settlementDays_;
        BusinessDayConvention businessDayConvention_;
        DayCounter dc_;
        boost::shared_ptr<IborIndex> iborIndex_;

        std::vector<Period> optionletTenors_;
        Size nOptionletTenors_;
        std::vector<Date> optionletDates_;
        std::vector<Time> optionletTimes_;
        mutable std::vector<Rate> optionletAtmRates_;
        std::vector<std::vector<Rate> > optionletStrikes_;
        Size nStrikes_; 

        std::vector<std::vector<Handle<Quote> > > optionletVolQuotes_;
        mutable std::vector<std::vector<Volatility> > optionletVolatilities_;
    };

}

#endif