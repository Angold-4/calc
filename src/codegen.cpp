#include "calc.hpp"

namespace calc {
    /* Multi-target Support (Mac, Linux)*/
    void Generator::GenCode(Node *node) {
#ifdef MAC
        prologue_mac();
#endif    // MAC

#ifdef LINUX
        prologue_linux();
#endif    // LINUX

        start = node;
        GenASM(node);

        printf("  ret\n");
        assert(depth == 0);
        return;
    }

    /* After each iteration of GenASM, it will put the
     * current value in the %rax */
    void Generator::GenASM(Node* node) {
        switch(node->kind) {
            /* Termination Condition ND_NUM */
            case ND_NUM:
                printf("  mov $%d, %%rax\n", node->val);
                return;

                /* Termination Condition ND_NEG */
            case ND_NEG:
                GenASM(node->lhs);
                printf("  neg %%rax\n");
                return;

            default:
                break;
        }

        GenASM(node->rhs);
        push();            /* store the current rax */
        GenASM(node->lhs);
        pop("%rdi");       /* rhs -> %rdi, lhs -> %rax */

        /* then binary expression, deal with this two regs */
        switch(node->kind) {
            case ND_ADD:
                printf("  add %%rdi, %%rax\n");
                return;
            case ND_SUB:
                printf("  sub %%rdi, %%rax\n");
                return;
            case ND_MUL:
                printf("  imul %%rdi, %%rax\n");
                return;
            case ND_DIV:
                printf("  cqo\n");   /* sign-extended */
                printf("  idiv %%rdi\n");
                return;
            default:
                error("invalid expression");
        }

        error("codegen: invalid expression");
    };


    /* store the current value %rax into stack */
    void Generator::push() {
        printf("  push %%rax\n");
        depth++;
        return;
    };

    /* get the last stored value in the stack, and put
     * it into reg */
    void Generator::pop(const char* reg) {
        printf("  pop %s\n", reg);
        depth--;
        return;
    };

    void Generator::prologue_mac() {
        printf(".section  __TEXT,__text,regular,pure_instructions\n");
        printf(".build_version macos, 12, 0	sdk_version 12, 0\n");
        printf(".globl _main\n");
        printf("_main:\n");
    };

    void Generator::prologue_linux() {
        printf(".globl main\n");
        printf("main:\n");
    }

} // namespace calc
