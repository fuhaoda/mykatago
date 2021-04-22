from board import Board

a = Board(3)

a.play(1,a.loc(0,0))
a.play(1,a.loc(1,0))
a.play(1,a.loc(2,0))
a.play(1,a.loc(1,2))
a.play(1,a.loc(2,2))
a.play(1,a.loc(2,1))

print(a.to_string())
print("\n\n")
print(a.to_liberty_string())

print("\n\n")
a.play(1,a.loc(2,1))
a.play(1,a.loc(1,2))
print(a.to_string())
print("\n\n")
print(a.to_liberty_string())