use cxx::{type_id, ExternType};

#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("rev2-mod/include/Interop.hpp");

        type TArray;
        type FString;

        fn find_globals() -> bool;
        fn to_string(string: &String) -> &FString;
        fn from_string(string: &FString) -> String;
    }
}