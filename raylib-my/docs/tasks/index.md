# Architecture Improvement Tasks

This document lists all identified improvements for the project architecture.

## Priority Levels
- 🔴 **Critical**: Must fix - affects compilation, safety, or correctness
- 🟡 **High**: Should fix - significant design improvements
- 🟢 **Medium**: Nice to have - quality improvements
- 🔵 **Low**: Optional - optimization or future extensibility

---

## Critical Priority 🔴

| ID | Task | Status | Priority |
|----|------|--------|----------|
| [TASK-01](task-01-cpp-includes.md) | Replace .cpp includes with proper headers | ✅ DONE | 🔴 Critical |
| [TASK-02](task-02-memory-management.md) | Fix manual memory management | ✅ DONE | 🔴 Critical |

## High Priority 🟡

| ID | Task | Status | Priority |
|----|------|--------|----------|
| [TASK-03](task-03-gameobject-interface.md) | Split GameObject interface | ✅ OBSOLETE | 🟡 High |
| [TASK-04](task-04-component-ownership.md) | Clarify component ownership | ✅ DONE | 🟡 High |
| [TASK-05](task-05-abstract-raylib.md) | Abstract raylib dependencies | 📋 TODO | 🟡 High |
| [TASK-06](task-06-error-handling.md) | Define error handling strategy | 📋 TODO | 🟡 High |

## Medium Priority 🟢

| ID | Task | Status | Priority |
|----|------|--------|----------|
| [TASK-07](task-07-tilesmanager-pattern.md) | Improve TilesManager pattern | 📋 TODO | 🟢 Medium |
| [TASK-08](task-08-state-separation.md) | Separate game state from view state | 📋 TODO | 🟢 Medium |
| [TASK-09](task-09-configuration.md) | Extract hard-coded dependencies | 📋 TODO | 🟢 Medium |
| [TASK-10](task-10-input-consumption.md) | Improve input handling pattern | 📋 TODO | 🟢 Medium |
| [TASK-11](task-11-abstraction-layers.md) | Add missing abstraction layers | 📋 TODO | 🟢 Medium |
| [TASK-12](task-12-extensibility.md) | Improve extensibility | 📋 TODO | 🟢 Medium |

## Low Priority 🔵

| ID | Task | Status | Priority |
|----|------|--------|----------|
| [TASK-13](task-13-sorting-optimization.md) | Optimize gameAreas sorting | 📋 TODO | 🔵 Low |
| [TASK-14](task-14-spatial-partitioning.md) | Add spatial partitioning | 📋 TODO | 🔵 Low |

---

## Task Status Legend
- 📋 TODO: Not started
- 🚧 IN PROGRESS: Currently being worked on
- ✅ DONE: Completed
- ✅ OBSOLETE: Already addressed by current implementation
- ❌ CANCELLED: No longer relevant

## Recommended Implementation Order

1. **Phase 1 - Foundation** (Critical fixes)
   - TASK-01: Fix .cpp includes
   - TASK-02: Fix memory management

2. **Phase 2 - Architecture** (High priority design)
   - ~~TASK-03: Split GameObject interface~~ (Obsolete - component-based architecture already addresses this)
   - ~~TASK-04: Component ownership~~ (Complete - menu ownership fixed)
   - TASK-05: Abstract raylib

3. **Phase 3 - Quality** (Medium priority improvements)
   - TASK-06: Error handling
   - TASK-08: State separation
   - TASK-09: Configuration
   - TASK-11: Abstraction layers

4. **Phase 4 - Extensibility** (Medium-Low priority)
   - TASK-07: TilesManager pattern
   - TASK-10: Input consumption
   - TASK-12: Extensibility

5. **Phase 5 - Optimization** (Low priority)
   - TASK-13: Sorting optimization
   - TASK-14: Spatial partitioning

---

## Quick Start

To begin working on a task:
1. Open the corresponding task file
2. Review the problem description and desired outcome
3. Update the status in this index
4. Implement the solution
5. Mark as completed when done

---

Last Updated: January 18, 2026
