#ifndef SOFTWARE1LIB_HFSP_H
#define SOFTWARE1LIB_HFSP_H

#include <stdint.h>
#include <bits/types/FILE.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    LeafNode = -1,
    IndexNode = 0,
    HeaderNode = 1,
    MapNode = 2
} hfsp_kind;

enum {
    kHFSPlusFolderRecord = 1,
    kHFSPlusFileRecord = 2,
    kHFSPlusFolderThreadRecord = 3,
    kHFSPlusFileThreadRecord = 4
};

typedef struct {
    uint32_t fLink;
    uint32_t bLink;
    int8_t kind;
    uint8_t height;
    uint16_t numRecords;
    uint16_t reserved;
} hfsp_node_descriptor;

typedef struct {
    hfsp_node_descriptor *descriptor;
    void **records;
    uint16_t *record_pointers;
} hfsp_node;

typedef struct  {
    uint16_t    treeDepth;
    uint32_t    rootNode;
    uint32_t    leafRecords;
    uint32_t    firstLeafNode;
    uint32_t    lastLeafNode;
    uint16_t    nodeSize;
    uint16_t    maxKeyLength;
    uint32_t    totalNodes;
    uint32_t    freeNodes;
    uint16_t    reserved1;
    uint32_t    clumpSize;
    uint8_t     btreeType;
    uint8_t     keyCompareType;
    uint32_t    attributes;
    uint32_t    reserved3[16];
} hfsp_header_record;

typedef struct {
    hfsp_node *header_node;
    hfsp_node **leaf_nodes;
    uint32_t leaf_nodes_num;
    uint32_t no_leaf_nodes_num;
    
    uint64_t fd;
    uint32_t btree_offset;
} hfsp_btree;

typedef struct {
    uint16_t key_length;
    uint32_t parent_id;
    uint16_t sym_length;
    char *node_name;
} hfsp_catalog_key;



typedef enum {
    NO_SUCH_FILE_OR_DIR,
    IS_FILE,
    ALREADY_ON_TOP,
    NO_RESERVED_SPACE,
    BAD_ROOT_STRUCTURE
} ERROR;

ERROR ERRNO;

typedef struct {
    uint32_t startBlock;
    uint32_t blockCount;
} hfsp_extent_descriptor;

typedef struct {
    uint64_t logicalSize;
    uint32_t clumpSize;
    uint32_t totalBlocks;
    hfsp_extent_descriptor extents[8];
} hfsp_fork_data;

typedef uint32_t HFSCatalogNodeID;

typedef struct {
    uint16_t keyLength;
    uint8_t forkType;
    uint8_t pad;
    HFSCatalogNodeID fileID;
    uint32_t startBlock;
} hfsp_extent_key ;

typedef struct {
    uint16_t signature;
    uint16_t version;
    uint32_t attributes;
    uint32_t lastMountedVersion;
    uint32_t journalInfoBlock;
    uint32_t createDate;
    uint32_t modifyDate;
    uint32_t backupDate;
    uint32_t checkedDate;
    uint32_t fileCount;
    uint32_t folderCount;
    uint32_t blockSize;
    uint32_t totalBlocks;
    uint32_t freeBlocks;
    uint32_t nextAllocation;
    uint32_t rsrcClumpSize;
    uint32_t dataClumpSize;
    uint32_t nextCatalogID;
    uint32_t writeCount;
    uint64_t encodingsBitmap;
    uint32_t finderInfo[8];
//5 specialnih failov
    hfsp_fork_data *allocationFile;
    hfsp_fork_data *extentsFile;
    hfsp_fork_data *catalogFile;
    hfsp_fork_data *attributesFile;
    hfsp_fork_data *startupFile;
} hfsp_volume_header;

typedef struct {
    int16_t recordType;
    uint16_t flags;
    uint32_t valence;
    uint32_t folderID;
    uint32_t createDate;
    uint32_t contentModDate;
    uint32_t attributeModDate;
    uint32_t accessDate;
    uint32_t backupDate;
    uint32_t textEncoding;
    uint32_t reserved2;
} hfsp_catalog_folder;

typedef struct {
    int16_t recordType;
    uint16_t flags;
    uint32_t reserved1;
    uint32_t fileID;
    uint32_t createDate;
    uint32_t contentModDate;
    uint32_t attributeModDate;
    uint32_t accessDate;
    uint32_t backupDate;
    uint32_t textEncoding;
    uint32_t reserved2;
    hfsp_fork_data dataFork;
    hfsp_fork_data resourceFork;
} hfsp_catalog_file;

typedef struct {
    char *pwd;
    uint32_t pathFolderIds[500];
    uint32_t path_folder_depth;
    uint32_t current_folder_id;
    hfsp_catalog_folder *current_folder_info;
    hfsp_volume_header *vol_header;
    hfsp_btree *catalog_file;
} hfsplus;

//functions


#endif //SOFTWARE1LIB_HFSP_H
