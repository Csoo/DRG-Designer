add_executable(DRG_Designer
    src/main.cpp
    src/drg.cpp
    src/drgchapter.cpp
    src/icd11.cpp
    src/listmodel.cpp
    src/qmlmanager.cpp
    src/repository.cpp
    src/selection.cpp
    src/session.cpp
    src/treeitem.cpp
    src/treemodel.cpp
    include/drg.h
    include/drgchapter.h
    include/icd11.h
    include/listmodel.h
    include/qmlmanager.h
    include/repository.h
    include/selection.h
    include/session.h
    include/treeitem.h
    include/treemodel.h
)

target_link_libraries(DRG_Designer PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Qml
    Qt6::Sql
)

target_include_directories(DRG_Designer PRIVATE 
    .\\include
        
)