C26444: No unnamed RAII objects
 => Disable
 -> Rationale: many false warnings

C26472 Don't use static_cast for arithmetic conversios
 => Disable
 -> Rationale: can only be solved with gsl::narrow_cast

C26474 No implicit cast
 => Disable
 -> Rationale: many false warnings

C26489 Don't derence a pointer that may be invalid
 => Disable
 -> Rationale: many false warnings

C26492
 => Disable
 -> Rationale: many false warnings (va_start)

C26493 Don't use C-style casts

C26461 pointer to const
 => Disable
 -> Rationale: some difficult to fix false reports

C26496 variable is only assigned once, mark it const
 => Disable
 -> Rationale: false warnings, in combi with ATL message map macro's
               Resharper can do the same.

C26434 Hides a non-virtual function
 => Disable, 
 => Rationale: ATL template mechanism uses this technique
 
C26462: only assigned once
 => Disable, 
 => Rationale: false warnings, Resharper is better

C26440 can be declared noexcept
 => Disable, 
 => Rationale: false warnings, can be enabled for analysis purposes

C26439 special noexcep funtions
 => Disable, 
 => Rationale: 

