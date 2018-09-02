#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// Translate a postfix expression into an infix expression.
// Reads from stdin.

// Productions:
// a -> bn+|bn-|b
// b -> an*|an/|bn*|bn/|n
// n -> [0-9]
// Semantic rules:
// a-> b+n|b-n|b
// b->(a)n|(a)/n|b*n|b/n|n

struct ctx_t {
	char buf[100];
	unsigned len;

	char s[100];
	unsigned s_len;
};

typedef struct ctx_t ctx_t;

char pop(ctx_t *ctx) {
	char c = ctx->buf[ctx->len - 1];
	ctx->len -= 1;
	return c;
}

char peek(ctx_t *ctx) {
	return ctx->buf[ctx->len - 1];
}

void put(ctx_t *ctx, char c) {
	ctx->s[ctx->s_len] = c;
	ctx->s_len += 1;
}

void produce(ctx_t *ctx, int needs_param) {
	switch (peek(ctx)) {
	case '+':
	case '-':
	{
		char c = pop(ctx);

		if (needs_param)
			put(ctx, ')');
		produce(ctx, 0);
		put(ctx, c);
		produce(ctx, 0);
		if (needs_param)
			put(ctx, '(');
		break;
	}
	default:
	{
		char c = pop(ctx);

		if (isdigit(c))  {
			put(ctx, c);
		} else if (c == '*' || c == '/') {
			produce(ctx, 1);
			put(ctx, c);
			produce(ctx, 1);
		} else {
			fprintf(stderr, "Invalid char %c at pos %d\n", c, ctx->len);
			exit(1);
		}
		break;
	}
	}
}

int main(void) {
	ctx_t ctx;
	ctx.len = 0;
	ctx.s_len = 0;

	char c;
	while (c = getchar()) {
		if (c == '\n') {
			ctx.buf[ctx.len] = '\0';
			break;
		}
		ctx.buf[ctx.len] = c;
		++ctx.len;
	}

	if (ctx.len == 0) {
		return 0;
	}

	produce(&ctx, 0);

	for (unsigned i = 0; ctx.buf[i] != '\0'; ++i)
		printf("%c", ctx.buf[i]);

	printf(" -> ");
	for (unsigned i = ctx.s_len - 1; i > 0; --i)
		printf("%c", ctx.s[i]);
	printf("%c\n", ctx.s[0]);

	return 0;
 }

