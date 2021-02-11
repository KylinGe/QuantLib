/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006 Giorgio Facchinetti

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

#ifndef quantlib_multistep_swap_hpp
#define quantlib_multistep_swap_hpp

#include <ql/models/marketmodels/products/multiproductmultistep.hpp>

namespace QuantLib {

    // TODO: add payer/receiver choice

    class MultiStepSwap : public MultiProductMultiStep {
      public:
        MultiStepSwap(const std::vector<Time>& rateTimes,
                      std::vector<Real> fixedAccruals,
                      std::vector<Real> floatingAccruals,
                      const std::vector<Time>& paymentTimes,
                      Rate fixedRate,
                      bool payer = true);
        //! \name MarketModelMultiProduct interface
        //@{
        std::vector<Time> possibleCashFlowTimes() const override;
        Size numberOfProducts() const override;
        Size maxNumberOfCashFlowsPerProductPerStep() const override;
        void reset() override;
        bool nextTimeStep(const CurveState& currentState,
                          std::vector<Size>& numberCashFlowsThisStep,
                          std::vector<std::vector<CashFlow> >& cashFlowsGenerated) override;
#if defined(QL_USE_STD_UNIQUE_PTR)
        std::unique_ptr<MarketModelMultiProduct> clone() const override;
#else
        std::auto_ptr<MarketModelMultiProduct> clone() const;
        #endif
        //@}
      private:
        std::vector<Real> fixedAccruals_, floatingAccruals_;
        std::vector<Time> paymentTimes_;
        Rate fixedRate_;
        Real multiplier_;
        Size lastIndex_;
        // things that vary in a path
        Size currentIndex_;
    };


    // inline definitions

    inline std::vector<Time>
    MultiStepSwap::possibleCashFlowTimes() const {
        return paymentTimes_;
    }

    inline Size MultiStepSwap::numberOfProducts() const {
        return 1;
    }

    inline Size
    MultiStepSwap::maxNumberOfCashFlowsPerProductPerStep() const {
        return 2;
    }

    inline void MultiStepSwap::reset() {
       currentIndex_=0;
    }

}

#endif
