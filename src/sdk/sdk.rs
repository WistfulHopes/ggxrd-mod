use cxx::{type_id, ExternType};

#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("rev2-mod/include/Interop.hpp");

        type TArray;
        type FString;

        // library functions
        fn find_globals() -> bool;
        fn to_string(string: &String) -> &FString;
        fn from_string(string: &FString) -> String;

        // mod functions
        fn change_scene(scene_id: i32) -> i32;
    }
}