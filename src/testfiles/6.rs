fn return3Literals() -> (bool, i32, bool) {
	return false, 5, false;
}
let t1, t2, t3 = return3Literals();

fn printSomething() { let b = 999999999; println!(b); }

fn expReturnBool() -> (bool) { (false) }
let expReturnBool_ = expReturnBool();

fn expReturnInt() -> i32 { 
    5+155 
}
let expReturnInt_ = expReturnInt();

fn returnX(x: bool, y: bool) -> (bool, bool) { x, y }
let x, y = false, false;
let returnX_, returnY_ = returnX(x, y);

fn expReturnBoth() -> (bool, i32) { ((false, 5)) }
let expReturnBoth_1, expReturnBoth_2  = expReturnBoth();


fn recursive_fibonacci(n: i32)->(i32) {
	if n <= 1 {
		return n;
	} else {
		return recursive_fibonacci(n - 1) + recursive_fibonacci(n - 2);
	};
}


fn returnReversed3(n: i32, b: bool, f: i32) -> (i32, bool, i32) {
    return f,b,n;
}
let returnRevered_1, returnRevered_2, returnRevered_3 = returnReversed3(9000, true, 121312);

fn returnUnaryExp(n:i32) -> i32 {
    println!(n+51515);
    5+15151;    
}
fn main() {
    let bananas : i32 = 5; // niet mutable
    let mut bananas = bananas + 2;
    bananas+=1;
    println!(bananas, bananas);
    println!();
    printSomething();
    println!();
    println!(t1, t2, t3);
    println!();
    println!(expReturnBool_);
    println!();
    println!(expReturnInt_);
    println!();
    println!(expReturnBoth_1, expReturnBoth_2);
    println!();
    println!(returnX_, returnY_);
    println!();

    let condition = bananas > 5;
    println!(condition);

    println!();
    if bananas > 5 {
        println!(bananas);
    } else {
        println!(bananas, bananas);
    }

    println!();
    for mut test1 in 0..3 {
        println!(test1);
        test1 = test1 + 15;
        println!(test1);
        break;
        println!(test1);
    }

    println!();
    for fib in 1..10{
        println!(fib);
		println!(recursive_fibonacci(fib));
	};


    println!();
    let ((x), (y), z) = (10, true, 5);
    println!(x, y, z);
    
    println!();
    let mut ((x)) = ((10));
    x+= bananas + 5;
    println!(x);

    println!();
    println!(expReturnBoth_1, expReturnBoth_2);

    println!();
    println!(returnRevered_1, returnRevered_2, returnRevered_3);

    
    println!();
    let mut wh:i32 = 0;
    while wh < 5 {
        wh+=1;
        println!(wh);
    };

    println!(returnUnaryExp(5));
}
