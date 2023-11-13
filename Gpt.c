#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 100
#define MEMORY_SIZE 100

typedef struct {
    int stack[STACK_SIZE];
    int sp; // Stack pointer
    int memory[MEMORY_SIZE];
} StackMachine;

typedef enum {
    PUSH,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    DUP,
    SWAP,
    PRINT,
    JMP,
    JZ,
    JNZ,
    EQ,
    LT,
    GT,
    ALLOC,  // Allocate memory for a variable
    LOAD,   // Load a variable onto the stack
    STORE,  // Store the top of the stack into a variable
    HALT
} Instruction;

void push(StackMachine *machine, int value) {
    machine->stack[machine->sp++] = value;
}

int pop(StackMachine *machine) {
    return machine->stack[--machine->sp];
}

void execute(Instruction program[], int program_size) {
    StackMachine machine;
    machine.sp = 0;
    int ip = 0; // Instruction pointer

    while (ip < program_size) {
        Instruction current = program[ip];

        switch (current) {
            case PUSH:
                push(&machine, program[++ip]);
                break;
            case POP:
                pop(&machine);
                break;
            case ADD:
                push(&machine, pop(&machine) + pop(&machine));
                break;
            case SUB:
                push(&machine, -pop(&machine) + pop(&machine));
                break;
            case MUL:
                push(&machine, pop(&machine) * pop(&machine));
                break;
            case DIV:
                push(&machine, 1 / pop(&machine) * pop(&machine));
                break;
            case DUP:
                push(&machine, machine.stack[machine.sp - 1]);
                break;
            case SWAP:
                {
                    int temp = machine.stack[machine.sp - 1];
                    machine.stack[machine.sp - 1] = machine.stack[machine.sp - 2];
                    machine.stack[machine.sp - 2] = temp;
                }
                break;
            case PRINT:
                printf("%d\n", machine.stack[machine.sp - 1]);
                break;
            case JMP:
                ip = program[++ip];
                break;
            case JZ:
                if (pop(&machine) == 0) {
                    ip = program[++ip];
                } else {
                    ip += 2; // Skip the jump target
                }
                break;
            case JNZ:
                if (pop(&machine) != 0) {
                    ip = program[++ip];
                } else {
                    ip += 2; // Skip the jump target
                }
                break;
            case EQ:
                push(&machine, pop(&machine) == pop(&machine));
                break;
            case LT:
                push(&machine, pop(&machine) > pop(&machine));
                break;
            case GT:
                push(&machine, pop(&machine) < pop(&machine));
                break;
            case ALLOC:
                {
                    int var_index = program[++ip];
                    machine.memory[var_index] = 0;  // Initialize the variable
                }
                break;
            case LOAD:
                {
                    int var_index = program[++ip];
                    push(&machine, machine.memory[var_index]);
                }
                break;
            case STORE:
                {
                    int var_index = program[++ip];
                    machine.memory[var_index] = pop(&machine);
                }
                break;
            case HALT:
                return;
            default:
                printf("Unknown instruction\n");
                return;
        }

        ip++;
    }
}

int main() {
    // Example program: ALLOC 0, ALLOC 1, PUSH 2, STORE 0, PUSH 3, STORE 1, LOAD 0, LOAD 1, ADD, PRINT, HALT
    Instruction program[] = {
        ALLOC, 0,
        ALLOC, 1,
        PUSH, 2, STORE, 0,
        PUSH, 3, STORE, 1,
        LOAD, 0,
        LOAD, 1,
        ADD,
        PRINT,
        HALT
    };

    execute(program, sizeof(program) / sizeof(program[0]));

    return 0;
}
