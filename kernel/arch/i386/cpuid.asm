; Copyright 2014, 2015 Alejandro Bezdjian
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.

GLOBAL _cpuvendor
GLOBAL _cpufeatures1
GLOBAL _cpufeatures2

_cpuvendor:
    push    ebp
    mov     ebp, esp

    mov     eax, 0

    cpuid

    mov     eax, ebx

    mov     esp, ebp
    pop     ebp
    ret
