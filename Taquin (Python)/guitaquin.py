from taquin import *
import tkinter
import random

class VueTaquin:
    def __init__(self,taquin):
        self.__taquin = taquin
        dim = taquin.dim()
        fenetre = tkinter.Tk()
        fenetre.title("Taquin")
        cases = tkinter.Frame()
        self.__images = []
        rien = tkinter.PhotoImage(file="./alphabet/rien.gif")
        self.__images.append(rien)
        a = tkinter.PhotoImage(file="./alphabet/a.gif")
        self.__images.append(a)
        b = tkinter.PhotoImage(file="./alphabet/b.gif")
        self.__images.append(b)
        c = tkinter.PhotoImage(file="./alphabet/c.gif")
        self.__images.append(c)
        d = tkinter.PhotoImage(file="./alphabet/d.gif")
        self.__images.append(d)
        e = tkinter.PhotoImage(file="./alphabet/e.gif")
        self.__images.append(e)
        f = tkinter.PhotoImage(file="./alphabet/f.gif")
        self.__images.append(f)
        g = tkinter.PhotoImage(file="./alphabet/g.gif")
        self.__images.append(g)
        h = tkinter.PhotoImage(file="./alphabet/h.gif")
        self.__images.append(h)
        i = tkinter.PhotoImage(file="./alphabet/i.gif")
        self.__images.append(i)
        j = tkinter.PhotoImage(file="./alphabet/j.gif")
        self.__images.append(j)
        k = tkinter.PhotoImage(file="./alphabet/k.gif")
        self.__images.append(k)
        l = tkinter.PhotoImage(file="./alphabet/l.gif")
        self.__images.append(l)
        m = tkinter.PhotoImage(file="./alphabet/m.gif")
        self.__images.append(m)
        n = tkinter.PhotoImage(file="./alphabet/n.gif")
        self.__images.append(n)
        o = tkinter.PhotoImage(file="./alphabet/o.gif")
        self.__images.append(o)
        images = self.__images
        for x in range(dim):
            for y in range(dim):
                if (self.__taquin.get(x,y) == -1):
                    img = images[0]
                else:
                    img = images[self.__taquin.get(x,y)]
                bouton = tkinter.Button(cases,image=img)
                bouton.grid(row=x,column=y,sticky="NSEW")
        cases.pack()
        boutons = tkinter.Frame()
        btn_reinit = tkinter.Button(boutons,text="Recommencer")
        btn_reinit.grid(row=0,column=0,sticky="NSEW")
        btn_quitter = tkinter.Button(boutons,text="Au revoir",command=fenetre.destroy)
        btn_quitter.grid(row=0,column=1,sticky="NSEW")
        boutons.pack()
        fenetre.mainloop()


if __name__ == '__main__' :
    taquin = Taquin()
    taq = VueTaquin(taquin)