#pragma once

#include <IOTarget.hpp>
#include <queue>

template <typename T, CASK::AccessType accessType>
class TransactionBuffer {

private:

    CASK::IOTarget *target;

    struct TransactionCall {
        T startAddress;
        T size;
        char* buf;
    };

    std::queue<TransactionCall> transactionQueue;

public:

    TransactionBuffer(CASK::IOTarget* ioTarget) :
        target(ioTarget) {
    };

    constexpr T BufferedIO(T startAddress, T size, char* buf) {
        transactionQueue.push({startAddress, size, buf});
        return size;
    }

    T Commit() {
        T sizeSum = 0;
        while (!transactionQueue.empty()) {
            TransactionCall transaction = transactionQueue.front();
            transactionQueue.pop();
            T successSize = target->Transact<T, accessType>(transaction.startAddress, transaction.size, transaction.buf);
            sizeSum += successSize;
            if (successSize != transaction.size) {
                break;
            }
        }
        return sizeSum;
    }

};
