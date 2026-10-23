//
// Created by emi on 10/21/26.
//

#pragma once
#include <memory>

namespace Bedrock {
    class EnableNonOwnerReferences {
    public:
        // NOLINTBEGIN
        struct ControlBlock {
        public:
            bool mIsValid;
        };

        std::shared_ptr<Bedrock::EnableNonOwnerReferences::ControlBlock> mControlBlock;

        virtual ~EnableNonOwnerReferences();
        // NOLINTBEGIN
    };

};
