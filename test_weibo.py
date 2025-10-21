import subprocess
import time

def run_test():
    print("微博服务功能测试")
    print("=" * 30)
    
    # 编译项目
    print("1. 编译项目...")
    compile_result = subprocess.run(["g++", "-std=c++17", "*.cpp", "-o", "SimulatedCommunicationSystem.exe"], 
                                   cwd="D:\\vs2022\\PRO++\\SimulatedCommunicationSystem",
                                   capture_output=True, text=True)
    
    if compile_result.returncode != 0:
        print("编译失败:")
        print(compile_result.stderr)
        return
    
    print("编译成功!")
    
    # 运行程序并测试
    print("\n2. 运行程序测试...")
    try:
        # 启动程序
        process = subprocess.Popen(["./SimulatedCommunicationSystem.exe"], 
                                  cwd="D:\\vs2022\\PRO++\\SimulatedCommunicationSystem",
                                  stdout=subprocess.PIPE, 
                                  stderr=subprocess.PIPE,
                                  stdin=subprocess.PIPE,
                                  text=True)
        
        # 等待程序启动
        time.sleep(2)
        
        # 发送输入
        stdout, stderr = process.communicate(input="3\ntest_qq_user\nTestQQUser\n1990-01-01\nBeijing\nqq_password\n", 
                                           timeout=10)
        
        print("程序输出:")
        print(stdout)
        
        if stderr:
            print("错误信息:")
            print(stderr)
            
    except Exception as e:
        print(f"运行程序时出错: {e}")

if __name__ == "__main__":
    run_test()