# Qt 6 Migration Plan

## Overview

Bliss currently builds against Qt 5. This document tracks what needs to change for Qt 6.

## Required Changes

### 1. Replace `foreach` macro (~82 occurrences) — BLOCKING

The `foreach` (Q_FOREACH) macro is deprecated/removed in Qt 6. Replace with C++11 range-based `for` loops:

```cpp
// Before:
foreach(Type var, container) { ... }

// After:
for (const auto &var : container) { ... }
```

Affected files:
- `lib/bliss/bliss.cpp` (13 occurrences)
- `src/groupview.cpp` (19 occurrences)
- `src/mainmodel.cpp` (17 occurrences)
- `src/listitem.cpp` (8 occurrences)
- `src/groupadderitem.cpp` (3 occurrences)
- `src/fanmenu.cpp` (2 occurrences)
- `src/storagefile.cpp` (2 occurrences)
- `src/todohandleitem.cpp`, `src/droptargetitem.cpp`, `src/history.cpp`, `src/storagegit.cpp`, `src/itemplacer.cpp`, `src/historyview.cpp`, `src/todoitem.cpp` (1 each)

### 2. Modernize SIGNAL/SLOT macros (~154 occurrences) — RECOMMENDED

Old-style `SIGNAL()`/`SLOT()` macros still compile in Qt 6 but should be updated to the type-safe function pointer syntax:

```cpp
// Before:
connect(obj1, SIGNAL(signal()), obj2, SLOT(slot()));

// After:
connect(obj1, &Class::signal, obj2, &Class::slot);
```

This is spread across nearly all source files.

## No Issues Found (Compatible as-is)

- `QT += xml gui widgets` module declaration
- QGraphicsView/QGraphicsScene usage (still available in Qt 6, test for behavior changes)
- QList/QMap container usage (unified in Qt 6, transparent)
- QDomDocument/QXmlStreamWriter XML support
- No use of removed APIs: QDesktopWidget, QRegExp, QTextCodec, qSort, qFind

## Build with Qt 6

```bash
brew install qt
export PATH="/opt/homebrew/bin:$PATH"
qmake bliss.pro
make
```
