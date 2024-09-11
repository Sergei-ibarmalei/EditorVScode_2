#pragma once

constexpr int RowMax{ 32 };
constexpr int ColMax{ 32 };
constexpr int RowMin{ 4 };
constexpr int ColMin{ 4 };

// --- table
constexpr int workTableStartX{ 80 };
constexpr int workTableStartY{ 80 };
constexpr int workTableRowsCount{ 16 };
constexpr int workTableColsCount{ 16 };

constexpr int workPixelSide{ 30 };
constexpr int workPixelSpace{ 10 };

constexpr int colorPixelSide{ 15 };
constexpr int colorPixelSpace{ 5 };

constexpr int workColorSpaceBetween{ 50 };

constexpr int colorRowsCount{ 9 };
constexpr int colorColsCount{ 13 };

constexpr int colorTableStartX{ workTableStartX +
	(workTableColsCount * workPixelSide) + 
	(workTableColsCount - 1) * workPixelSpace +
	workColorSpaceBetween };
constexpr int colorTableStartY{ workTableStartY };

constexpr int workTableSide {(workTableRowsCount * workPixelSide) + 
	(workTableRowsCount - 1) * workPixelSpace};

