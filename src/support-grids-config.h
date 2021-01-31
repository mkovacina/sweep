#pragma once

#include "input-data-source.h"

#define MAX_GRIDS (25)

typedef enum
{
	Uniform,
	File,
	Random,
	Computed,
	Distributed,
	Proportional,
	Toss,
	RestrictedToss,
}
GridInitializationMethod;

typedef struct
{
	float InitializationValue;
}
UniformInitializationParameters;

#define MAX_FILEPATH_LENGTH (256)

typedef struct 
{
	char FilePath[MAX_FILEPATH_LENGTH];
}
FileInitializationParameters;

typedef struct
{
	float MinimumRandomValue;
	float MaximumRandomValue;
}
RandomInitializationParameters;

typedef struct
{
}
ComputedInitializationParameters;

#define MAX_DISTRIBUTED_VALUES (50)

typedef struct
{
	unsigned int NumberOfValues;
	float Values[MAX_DISTRIBUTED_VALUES];
}
DistributedInitializationParameters;

#define MAX_PROPORTIONAL_VALUES (30)

typedef struct
{
	unsigned int NumberOfValues;
	float Values[MAX_PROPORTIONAL_VALUES][2];
}
ProportionalInitializationParameters;

typedef struct
{
	unsigned int NumberOfValues;
	unsigned int ValueToPlace;
	unsigned int DefaultValue;
	unsigned int LowerLeftRow;
	unsigned int LowerLeftColumn;
	unsigned int UpperRightRow;
	unsigned int UpperRightColumn;
}
TossInitializationParameters;

typedef struct
{
	unsigned int NumberOfValues;
	unsigned int ValueToPlace;
	unsigned int DefaultValue;
	unsigned int LowerLeftRow;
	unsigned int LowerLeftColumn;
	unsigned int UpperRightRow;
	unsigned int UpperRightColumn;
	unsigned int HoleLowerLeftRow;
	unsigned int HoleLowerLeftColumn;
	unsigned int HoleUpperRightRow;
	unsigned int HoleUpperRightColumn;
}
RestrictedTossInitializationParameters;

typedef struct
{
}
StaticUpdateParameters;

typedef struct
{
}
ClearUpdateParameters;

typedef struct
{
	int RowOffest;
	int ColumnOffset;
}
MovementUpdateParameters;

typedef struct
{
	float DiffusionRate;
	float DecayRate;
}
DiffusionUpdateParameters;

typedef struct
{
}
CycleUpdateParameters;

typedef struct
{
}
LibraryUpdateParameters;

typedef struct
{
}
EUpdateParameters;

typedef enum
{
	Static,
	Clear,
	Move,
	Diffusion,
	Cycle,
	Library,
	E,
}
GridUpdateMethod;

typedef union
{
	UniformInitializationParameters Uniform;
	FileInitializationParameters File;
	RandomInitializationParameters Random;
	ComputedInitializationParameters Computed;
	DistributedInitializationParameters Distributed;
	ProportionalInitializationParameters Proportional;
	TossInitializationParameters Toss;
	RestrictedTossInitializationParameters RestrictedToss;
}
GridInitializationParameters;

typedef struct 
{
	unsigned int GridID;
	GridInitializationMethod InitializationMethod;
	GridInitializationParameters InitializationParameters;
	GridUpdateMethod UpdateMethod;
}
SupportGridDefinition;

typedef struct 
{
	unsigned int NumberOfSupportGrids;
	unsigned int NumberOfRows;
	unsigned int NumberOfColumns;
	SupportGridDefinition GridDefinitions[MAX_GRIDS];	
}
SupportGridConfiguration;

void ParseSupportGridConfig(InputDataSource* source);
