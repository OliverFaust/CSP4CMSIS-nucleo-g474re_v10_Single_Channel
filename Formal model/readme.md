# Process_Channel: Formal Model (CSP-M)

This directory contains the formal **Communicating Sequential Processes (CSP)** specification for a minimal multi-process network. The model mathematically defines a classic two-process communication pipeline operating under a zero-capacity, synchronous rendezvous protocol. The model was verified with the [ProB](https://prob.hhu.de/) model checker.

While the primary repository implements this pattern using C++ via the `CSP4CMSIS` library on a FreeRTOS kernel, this formal specification strips away RTOS preemption and hardware allocation mechanics to mathematically analyze and prove properties regarding concurrent synchronization.

## Network Architecture

The architecture models a localized producer-consumer architecture composed of two synchronized processes interacting across a single communication channel:

1. **`channel c`:** A zero-capacity rendezvous channel constrained to a data type domain of integers from `0` to `9`. In CSP semantics, communication occurs if and only if both the sender and receiver are simultaneously prepared to engage, forcing execution lockstep.
2. **`Sender(n)`:** A parameterized process representing a stateful entity. It maintains an internal counter `n`, outputs that value onto channel `c` (`c!n`), and then recursively invokes itself with an incremented, wrapped value.
3. **`Receiver`:** A data-independent process that blocks until data is available on channel `c`, accepts the transmitted value (`c?x`), and recurses back to its initial state to await the next transmission.

## CSP-M Specification

```csp
-- Process_Channel
channel c : {0..9}

Sender(n) = c!n -> Sender((n+1)%10)

Receiver =
    c?x -> Receiver

SYSTEM =  Sender(0) [| {|c|} |] Receiver
