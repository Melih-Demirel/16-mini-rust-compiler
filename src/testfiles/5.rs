// Bij literal mag ; niet.
fn blockTest1(n:i32) -> i32 {
    true
}
fn blockTest2(n:i32) -> i32 {
    n+5;
}
// Moet ; 
fn blockTest3(n:i32) -> i32 {
    let m = n;
    m+12;
}

fn blockTest4(n:i32) {
    println!(n);
}

fn main(){
    let blockTest_ = blockTest1(5);
    println!(blockTest_);
    let blockTest_ = blockTest2(5);
    println!(blockTest_);
    let blockTest_ = blockTest3(5);
    println!(blockTest_);

    blockTest4(99);

    // // No time for implementing this
    // let y =
    // {
    //     let x = 3;
    //     x + 1
    // };
    // println!(y);
}
