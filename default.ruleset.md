# Comments on disabled C++ Core Guidelines Rules

C26429: Use a not_null to indicate that "null" is not a valid value
-> Rationale: Prefast attributes are better.

C26434 Hides a non-virtual function
 => Rationale: ATL template mechanism uses this technique

C26440 can be declared noexcept
 => Rationale: generates false warnings, can be enabled for analysis purposes.

C26446: Prefer to use gsl::at() instead of unchecked subscript operator.
 -> Rationale: gsl:at() cannot be used. debug STL already checks.

C26447: The function is declared noexcept but calls a function that may throw exceptions.
 -> Rationale: Too many false warnings, caused by ATL and SDK functions not defined as noexcept.

C26466: Don't use static_cast downcasts. A cast from a polymorphic type should use dynamic_cast.
 -> Rationale: not using RTTI to support dynamic_cast.

C26472: Don't use static_cast for arithmetic conversions
 -> Rationale: can only be solved with gsl::narrow_cast

C26474: No implicit cast
-> Rationale: many false warnings

C26477: Don't use NULL or 0 but nullptr
-> Rationale: many false warnings in ATL macro's.

C26481: Do not pass an array as a single pointer.
-> Rationale: gsl::span is not available.

C26482: Only index into arrays using constant expressions.
-> Rationale: static analysis can verify access.

C26485: Do not pass an array as a single pointer
-> Rationale: see C26481.

C26486: Lifetime problem.
-> Rationale: many false warnings. (VS 2019 16.0.0 Preview 4.1)

C26487: Don't return a pointer that may be invalid (lifetime.4).
-> Rationale: many false warnings (VS 2019 16.0.0 Preview 1.1)

C26489: Don't derence a pointer that may be invalid
-> Rationale: many false warnings

C26490: Don't use reinterpret_cast
-> Rationale: required to work with win32 API

C26494: Variable 'x' is uninitialized. Always initialize an object
-> Rationale: many false warnings, other analyzers are better.

C26812: The enum type '...' is unscoped. Prefer 'enum class' over 'enum' (Enum.3).
-> Rationale: false warnings generated for enums from Windows SDK (VS 2019 16.3.0 Preview 3)
