i=0
sum = 0
L_START:
if i >= 5 goto L_END
t1 = sum + i
sum = t1
t2 = i + 1 i
= t2
goto L_START
L_END:
if sum != 10 goto L_ELSE
result = 1
goto L_END_IF
L_ELSE:
result = 0
L_END_IF