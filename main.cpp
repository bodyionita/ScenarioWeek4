#include <bits/stdc++.h>
#define fisierIntrare "robots.mat"
#define fisierIesire "solution.txt"
#define fisierTranzitie "intermediar.txt"
#define fisierRoboti "polyrobots.txt"
#define fisierPoly "polygon.txt"
using namespace std;
const char username[] = "thunderbird";
const char password[] = "vp9jg6li299peb5f94f2k79raq";
const double epsilon = 0.00000000001;
const int numar_teste = 30;
struct comparator
{
    bool operator()(int i, int j)
    {
        return i > j;
    }
};
struct point
{
    double x;
    double y;
    bool operator<(const point& rhs) const
    {
        return rhs.x < this->x || (fabs(rhs.x - this->x) < epsilon && rhs.y < this->y);
    }
    bool operator>(const point& rhs) const
    {
        return rhs.x > this->x || (fabs(rhs.x - this->x) < epsilon && rhs.y > this->y);
    }

    point(double a = 0, double b = 0) : x(a), y(b) {}
    double distance(point a)
    {
        return (double)pow(((a.x-this->x)*(a.x-this->x) +(a.y-this->y)*(a.y-this->y)), 0.5);
    }
};

struct line
{
    double a;
    double b;
    double c;
    line (double x = 0, double y = 0, double z = 0): a(x), b(y), c(z) {}
    double panta()
    {
        if (this->b==0) return 999999;
        return (-1)*this->a/this->b;
    }
    double panta2()
    {
        if (this->b==0) return 999999;
        return (-1)*this->c/this->b;
    }
    point intersectie(line n)
    {
        if (this->a*n.b - this->b*n.a!=0)
        {
            double x = (this->b*n.c - this->c*n.b) / (this->a*n.b - this->b*n.a);
            double y = (this->a*n.c - this->c*n.a) / (this->b*n.a - this->a*n.b);
            return point(x,y);
        }
        return point(100000,100000);
    }
    line dreapta2Puncte(point n, point m)
    {
        double d,e,f;
        d=n.y - m.y;
        e=m.x - n.x;
        f=n.x*(m.y-n.y) - n.y*(m.x - n.x);
        return line(d,e,f);
    }

};
struct segment
{
    point a;
    point b;
    int father;
    int indice;
    segment(point x = point(0,0) , point y = point(0,0) , int parinte=0, int i=0) :
        a(x), b(y), father(parinte), indice(i) {}
};
struct distanta
{
    double dist;
    vector<point> path;
};


fstream fileIn, fileOut;
fstream fileRoboti, filePoly;
vector<point> poly[500];
double perimetru[500];
point P[405];
segment S[10000];
bool folositS[10000];
distanta D[500][500];
vector<point> Solutie[405];

int numberPoints, numberPoly, numberSegments;
int i, j, k;

void citire();
void clean();
point* calculeaza_distanta(point, point);
point* calculeaza_distanta2(point, point);
void clearDistante();
void preprocesare_distante();
void preprocesare_segmente();
void creare_solutieOriginala();
void creare_solutie();
void creare_solutie2();
void creare_solutie3();
void creare_solutie4();
void baga(int,int,int);
void clearSolutie();
void afis();
void initializeazaFolositS();
bool intersectie_segment(point,point,point,point);
double distance2(point, point);
void curata_output();
bool intalneste_segment(point, point);
int main()
{
    /// Clean input
    clean();

    fileIn.open(fisierTranzitie, fstream::in);
    fileOut.open(fisierIesire, fstream::out);
    fileRoboti.open(fisierRoboti, fstream::out);
    filePoly.open(fisierPoly, fstream::out);
    fileOut<<setprecision(17);
    fileOut<<username<<'\n'<<password<<'\n';
    for (int i=1; i<=numar_teste; i++)
    {
        /// Citire
        citire();
        if (i==numar_teste) numberPoints--;
        for (int k=1; k<=numberPoints; k++)
            fileRoboti<<setprecision(17)<<P[k].x<<" "<<P[k].y<<" ";
        fileRoboti<<'\n';
        for (int k=1; k<=numberPoly; k++)
        {
            for (int j=0; j<poly[k].size(); j++)
                filePoly<<poly[k][j].x<<" "<<poly[k][j].y<<" ";
            if (k<numberPoly)  filePoly<<"999999999 999999999 ";
        }
        filePoly<<'\n';

        /// Preprocesare segmente
        preprocesare_segmente();

        /// Preprocesare distante
        clearDistante();
        preprocesare_distante();

        /// Creare solutie. Trecem Robotul 1 prin toti ceilalti
        clearSolutie();
        creare_solutie3();

        ///Afiseaza solutie

        fileOut<<i<<": ";
        afis();

    }
    fileIn.close();
    fileOut.close();
    fileRoboti.close();
    filePoly.close();

    // curata_output();
    return 0;
}

bool intalneste_segment(point p1, point p2)
{
    for (int i=1; i<=numberSegments; i++)
    {
        line d1 = line(0,0,0).dreapta2Puncte(p1,p2);
        line d2 = line(0,0,0).dreapta2Puncte(S[i].a, S[i].b);
        point intersection = d1.intersectie(d2);
        bool apartineA = false;
        bool apartineB = false;
            if  (
                ((S[i].a.x<=intersection.x && intersection.x<=S[i].b.x) || (S[i].a.x>=intersection.x && intersection.x>=S[i].b.x))
                &&
                ((S[i].a.y<=intersection.y && intersection.y<=S[i].b.y) || (S[i].a.y>=intersection.y && intersection.y>=S[i].b.y))
            ) apartineB = true;
            if  (
                ((p1.x<intersection.x && intersection.x<p2.x) || (p1.x>intersection.x && intersection.x>p2.x))
                &&
                ((p1.y<intersection.y && intersection.y<p2.y) || (p2.y>intersection.y && intersection.y>p2.y))
                &&
                !(fabs(p1.x-intersection.x)<epsilon && fabs(p1.y-intersection.y)<epsilon)
                ) apartineA = true;
        if (apartineA && apartineB) return true;
    }
    return false;
}

void baga(int robot, int primul, int doi)
{
    for(int k=0; k<D[primul][doi].path.size(); k++)
        Solutie[robot].push_back(D[primul][doi].path[k]);
}
void creare_solutie4()
{
    vector< int > Q;

    bool Activat[500];
    int Locatie[500];
    memset(Activat, false, sizeof(Activat));
    memset(Locatie, 0, sizeof(Locatie));

    int remaining = numberPoints-1;
    Activat[1] = true;
    Locatie[1] = 1;
    Q.push_back( 1 );

    while (remaining > 0)
    {
        int candidat;
        int incoada;
        double minimum = 1000000;
        int catreCine;
        ///Cauta care robot va ajugne primul la destiantie
        for (int i=0; i<Q.size(); i++)
            for (int j=1; j<=numberPoints; j++)
                if (j!=Q[i] && !Activat[j])
                    if (D[Locatie[Q[i]]][j].path.size()<minimum)
                    {
                        minimum = D[Locatie[Q[i]]][j].path.size();
                        incoada = i;
                        candidat = Q[i];
                        catreCine = j;
                    }
                    ///asta am adaugat eu:
                    /*else if (D[Locatie[Q[i]]][j].path.size()==minimum)
                                if(D[Locatie[Q[i]]][j].dist<D[Locatie[Q[i]]][catreCine].dist)
                    {
                        incoada = i;
                        candidat = Q[i];
                        catreCine = j;
                    }*/

        Q.erase(Q.begin()+incoada);
        /// Cauta la care robot sa ma duc si anume ala spre care fac numar minim
        /// de pasi
        Q.push_back( catreCine);
        Q.push_back( candidat);
        Activat[catreCine] = true;

        for (int i=0; i<D[Locatie[candidat]][catreCine].path.size(); i++)
            Solutie[candidat].push_back(D[Locatie[candidat]][catreCine].path[i]);


        Locatie[candidat] = catreCine;
        Locatie[catreCine] = catreCine;
        remaining--;

    }


}

void creare_solutie()
{
    vector< pair <double, int> > Q;

    bool Activat[500];
    int Locatie[500];
    memset(Activat, false, sizeof(Activat));
    memset(Locatie, 0, sizeof(Locatie));

    int remaining = numberPoints-1;
    Activat[1] = true;
    Locatie[1] = 1;
    Q.push_back(make_pair(0, 1));

    while (remaining > 0)
    {
        int candidat;
        double minimum = 1000000;

        ///Cauta care robot va ajugne primul la destiantie
        for (int i=0; i<Q.size(); i++)
            if (Q[i].first < minimum)
            {
                candidat = Q[i].second;
                minimum = Q[i].first;
            }
        /// Scoate robotu care a ajuns la destiantie si scade "timpul" sau
        /// din toti ceilalti
        for (int i=0; i<Q.size(); i++)
        {
            Q[i].first-= minimum;
            if (Q[i].first < epsilon)
            {
                Q.erase(Q.begin()+i);
                break;
            }
        }
        /// Cauta la care robot sa ma duc si anume ala spre care fac numar minim
        /// de pasi
        int pasiMinimi = 100000;
        int catreCine = 0;
        for (int i=1; i<=numberPoints; i++)
            if (i!=candidat && !Activat[i])
                if (D[Locatie[candidat]][i].path.size() < pasiMinimi)
                {
                    pasiMinimi = D[Locatie[candidat]][i].path.size();
                    catreCine = i;
                }
        Q.push_back(make_pair(D[Locatie[candidat]][catreCine].dist, catreCine));
        Q.push_back(make_pair(D[Locatie[candidat]][catreCine].dist, candidat));
        Activat[catreCine] = true;

        for (int i=0; i<D[Locatie[candidat]][catreCine].path.size(); i++)
            Solutie[candidat].push_back(D[Locatie[candidat]][catreCine].path[i]);


        Locatie[candidat] = catreCine;
        Locatie[catreCine] = catreCine;
        remaining--;

    }

}

void creare_solutie2()
{
    ///ATENTIE CA D[i][j] poate exista si D[j][i] nu
    ///BOGDANE FA TI MAGIA SI FA SI DRUMURILE DE LA J LA I
    ///poti face asta si in alta parte, sau daca vrei sa salvezi timp,
    ///mai jos in codul meu daca vezi ca o distanta finala e buna,
    ///ii calculezi D[j][i] daca e nevoie, am marcat eu unde

    int pos[numberPoints+2];
    int des[numberPoints+2];
    for(int k=2; k<=numberPoints; k++) pos[k]=k, des[k]=0;
    pos[1]=1;
    des[1]=1;
    int n=numberPoints-1;

    ///trebuie sa fac o coada in care am doar robotiul 1 initial
    queue<int> q;
    q.push(1);

    while(n)
    {

        ///scot din coada un nr k
        int k;
        k=q.front();
        q.pop();

        ///aflu cel mai apropiat robot netrezit k2, fata de pos[k]
        int k2=0;
        int distLoc=9999999;
        for(int index=1; index<=numberPoints; index++)
        {
            if(index!=pos[k])
                if(des[index]==0)
                    if(D[index][pos[k]].dist < distLoc)
                    {
                        distLoc=D[index][pos[k]].dist;
                        k2=index;
                    }
        }

        ///AICI DACA pos[k]>k2, TREBUIE SA COMPUTAM D[pos[k]][k2];

        ///duc k in k2
        baga(k, pos[k], k2);
        pos[k]=k2;

        ///trezeste k2;
        des[k2]=1;

        ///baga k si k2 in coada la final ca sa trezesti si alti roboti cu ei
        q.push(k);
        q.push(k2);
        ///zi ca ai mai trezit un robot
        n--;
    }
}

void creare_solutie3()
{
    vector< pair <double, int> > Q;

    bool Activat[500];
    int Locatie[500];
    memset(Activat, false, sizeof(Activat));
    memset(Locatie, 0, sizeof(Locatie));

    int remaining = numberPoints-1;
    Activat[1] = true;
    Locatie[1] = 1;
    Q.push_back(make_pair(0, 1));

    while (remaining > 0)
    {
        int candidat;
        double minimum = 1000000;

        for (int i=0; i<Q.size(); i++)
            if (Q[i].first < minimum)
            {
                candidat = Q[i].second;
                minimum = Q[i].first;
            }
        for (int i=0; i<Q.size(); i++)
        {
            Q[i].first-= minimum;
            if (Q[i].first < epsilon)
            {
                Q.erase(Q.begin()+i);
                break;
            }
        }

        double distantaMinima = 100000;
        int catreCine = 0;
        for (int i=1; i<=numberPoints; i++)
            if (i!=candidat && !Activat[i])
                if (D[Locatie[candidat]][i].dist < distantaMinima)
                {
                    distantaMinima = D[Locatie[candidat]][i].dist;
                    catreCine = i;
                }
        Q.push_back(make_pair(D[Locatie[candidat]][catreCine].dist, catreCine));
        Q.push_back(make_pair(D[Locatie[candidat]][catreCine].dist, candidat));
        Activat[catreCine] = true;

        for (int i=0; i<D[Locatie[candidat]][catreCine].path.size(); i++)
            Solutie[candidat].push_back(D[Locatie[candidat]][catreCine].path[i]);


        Locatie[candidat] = catreCine;
        Locatie[catreCine] = catreCine;
        remaining--;

    }

}
void afis()
{
    int cateAuPathuri = 0;
    for (int i=1; i<=numberPoints; i++)
    {
        bool printat = false;
        bool trecut = false;
        for (int j=0; j<(int)(Solutie[i].size())-1; j++)
        {
            if (cateAuPathuri>0 && !printat)
            {
                fileOut<<"; ";
                printat = true;

            }

            if (fabs(Solutie[i][j].x-Solutie[i][j+1].x)>epsilon || fabs(Solutie[i][j].y-Solutie[i][j+1].y)>epsilon)
            {
                trecut=true;
                fileOut<<'('<<Solutie[i][j].x<<", "<<Solutie[i][j].y<<"), ";
            }
        }
        if (trecut) cateAuPathuri++;

        if (Solutie[i].size()) fileOut<<'('<<Solutie[i][Solutie[i].size()-1].x<<", "<<Solutie[i][Solutie[i].size()-1].y<<")";
    }
    fileOut<<'\n';
}

void clearSolutie()
{
    for (int i=1; i<=numberPoints; i++)
        Solutie[i].clear();
}

void initializeazaFolositS()
{
    for (int i=1; i<=numberSegments; i++)
        folositS[i] = false;
}

void creare_solutieOriginala()
{
    for (int j=2; j<=numberPoints; j++)
    {
        for (int k=0; k<D[j-1][j].path.size(); k++)
            Solutie[1].push_back(D[j-1][j].path[k]);
    }
}

bool intersectie_segment(point a1, point a2, point b1, point b2)
{
    line d1 = line(0,0,0).dreapta2Puncte(a1,a2);
    line d2 = line(0,0,0).dreapta2Puncte(b1,b2);
    point intersection = d1.intersectie(d2);
    bool apartineA = false;
    bool apartineB = false;
    if (( (a1.x <= intersection.x && intersection.x <= a2.x)||
            (a1.x >= intersection.x && intersection.x >= a2.x) ) &&
            ( (a1.y <= intersection.y && intersection.y <= a2.y)||
              (a1.y >= intersection.y && intersection.y >= a2.y) )&&
            (!(a1.x == intersection.x && a1.y == intersection.y)) &&
            (!(a2.x == intersection.x && a2.y == intersection.y))
       ) apartineA = true;

    if (( (b1.x <= intersection.x && intersection.x <= b2.x)||
            (b1.x >= intersection.x && intersection.x >= b2.x) ) &&
            ( (b1.y <= intersection.y && intersection.y <= b2.y)||
              (b1.y >= intersection.y && intersection.y >= b2.y) ) &&
            (!(b1.x == intersection.x && b1.y == intersection.y)) &&
            (!(b2.x == intersection.x && b2.y == intersection.y))
       ) apartineB = true;
    return (apartineA && apartineB);
}

void preprocesare_segmente()
{
    numberSegments = 0;
    for (int i=1; i<=numberPoly; i++)
    {
        for (int j=1; j<poly[i].size(); j++)
        {
            numberSegments++;
            S[numberSegments] = segment(poly[i][j-1], poly[i][j], i, numberSegments);
            perimetru[i]+= S[numberSegments].a.distance(S[numberSegments].b);
        }
        if (poly[i].size())
        {
            numberSegments++;
            S[numberSegments] = segment(poly[i][poly[i].size()-1], poly[i][0], i, numberSegments);
            perimetru[i]+= S[numberSegments].a.distance(S[numberSegments].b);
        }
    }
}

double distance2(point a, point b)
{
    double dx = (a.x-b.x)*(a.x-b.x);
    double dy = (a.y-b.y)*(a.y-b.y);
    return sqrt(dx+dy);
}

point* calculeaza_distanta(point p1, point p2)
{
    point Sol[10000];
    Sol[0].x = 1;
    Sol[1] = p1;

    point punctActual = p1;
    int poligonActual = 0;
    int segmentActual=0;
    bool amFostPePoligon = false;
    while (fabs(punctActual.x-p2.x) > epsilon || fabs(punctActual.y - p2.y)>epsilon)
    {
        bool asta = false;
        bool altul = false;
        double distanta = 100000;
        double celMaiDepartat = 0;
        segment segmentDepartat =segment();
        point punctDepartat;
        int poligonPosibil;
        folositS[segmentActual] = true;
        point punctPosibil;
        point urmatorulPunctPeContur;
        for (int i=1; i<=numberSegments; i++)
        {
            line d1 = line(0,0,0).dreapta2Puncte(punctActual,p2);
            line d2 = line(0,0,0).dreapta2Puncte(S[i].a, S[i].b);
            point intersection = d1.intersectie(d2);
            bool apartineA = false;
            bool apartineB = false;
            if  (
                ((S[i].a.x<=intersection.x && intersection.x<=S[i].b.x) || (S[i].a.x>=intersection.x && intersection.x>=S[i].b.x))
                &&
                ((S[i].a.y<=intersection.y && intersection.y<=S[i].b.y) || (S[i].a.y>=intersection.y && intersection.y>=S[i].b.y))
            ) apartineB = true;
            if  (
                ((punctActual.x<=intersection.x && intersection.x<=p2.x) || (punctActual.x>=intersection.x && intersection.x>=p2.x))
                &&
                ((punctActual.y<=intersection.y && intersection.y<=p2.y) || (punctActual.y>=intersection.y && intersection.y>=p2.y))
                &&
                !(fabs(punctActual.x-intersection.x)<epsilon && fabs(punctActual.y-intersection.y)<epsilon)
            ) apartineA = true;


            if (apartineA && apartineB)
            {
                if (S[i].father == poligonActual)
                {
                    asta = true;
                    if (punctActual.distance(intersection)> celMaiDepartat)
                    {
                        celMaiDepartat = punctActual.distance(intersection);
                        punctDepartat = intersection;
                        segmentDepartat = S[i];
                    }

                }
                else
                {
                    double dis = distance2(punctActual,intersection);

                    if (dis < distanta)
                    {
                        distanta = dis;
                        segmentActual = i;
                        punctPosibil = intersection;
                        poligonPosibil = S[i].father;
                    }
                    altul = true;
                }
            }
            bool intreX =(S[i].a.x<=punctActual.x && punctActual.x<=S[i].b.x) || (S[i].a.x>=punctActual.x && punctActual.x>=S[i].b.x);

            bool intreY =(S[i].a.y<=punctActual.y && punctActual.y<=S[i].b.y) || (S[i].a.y>=punctActual.y && punctActual.y>=S[i].b.y);
            bool capatA =fabs(punctActual.x-S[i].a.x)<epsilon && fabs(punctActual.y-S[i].a.y)<epsilon;
            line laturaPosibila = line(0,0,0).dreapta2Puncte(S[i].a, S[i].b);
            bool apartineLiniei = fabs(laturaPosibila.a*punctActual.x + laturaPosibila.b*punctActual.y + laturaPosibila.c) < epsilon;
            if  (intreX && intreY && !capatA && apartineLiniei  )
            {
                urmatorulPunctPeContur = S[i].a;
                segmentActual = i;
            }


        }
        if (asta)
        {
            /// mergi pe contur
            amFostPePoligon = true;
            punctActual = urmatorulPunctPeContur;
            int indice;
            for (indice=0; indice < poly[poligonActual].size(); indice++)
            {
                if (fabs(poly[poligonActual][indice].x - punctActual.x) < epsilon &&
                        fabs(poly[poligonActual][indice].y - punctActual.y) < epsilon) break;
            }
            //sol local;
            point Sol1[10000];
            double perimetruLocal = 0;
            bool gasit = false;
            //vector<point> *pol; //adresa catre poligonul in care ma uit;
            //pol = &poly[poligonActual];
            int N = poly[poligonActual].size();
            while (!gasit)
            {
                punctActual = poly[poligonActual][indice];
                Sol1[0].x++;
                Sol1[(int)Sol1[0].x] = punctActual;

                int indiceUrmator = (indice+N-1)%N;
                point punctUrmator= poly[poligonActual][indiceUrmator];
                line segmentUrmator = line(0,0,0).dreapta2Puncte(punctActual, punctUrmator);
                bool intreX =(punctActual.x<=punctDepartat.x && punctDepartat.x<=punctUrmator.x) || (punctActual.x>=punctDepartat.x && punctDepartat.x>=punctUrmator.x);
                bool intreY =(punctActual.y<=punctDepartat.y && punctDepartat.y<=punctUrmator.y) || (punctActual.y>=punctDepartat.y && punctDepartat.y>=punctUrmator.y);
                bool apartineLiniei = fabs(segmentUrmator.a*punctDepartat.x + segmentUrmator.b*punctDepartat.y + segmentUrmator.c) < epsilon;
                if (intreX && intreY && apartineLiniei)
                {
                    gasit = true;
                    Sol1[0].x++;
                    Sol1[(int)Sol1[0].x] = punctDepartat;
                    perimetruLocal += punctActual.distance(punctDepartat);
                    punctActual = punctDepartat;
                } else {perimetruLocal += punctActual.distance(punctUrmator);}

                indice = indiceUrmator;
            }
            if (perimetruLocal > perimetru[poligonActual]/2)
            {
                Sol1[0].x = 0;
                punctActual = urmatorulPunctPeContur;
                int indice;
                for (indice=0; indice < poly[poligonActual].size(); indice++)
                {
                    if (fabs(poly[poligonActual][indice].x - punctActual.x) < epsilon &&
                            fabs(poly[poligonActual][indice].y - punctActual.y) < epsilon) break;
                }
                int N = poly[poligonActual].size();
                bool gasit = false;
                while (!gasit)
                {
                    punctActual = poly[poligonActual][indice];
                    Sol1[0].x++;
                    Sol1[(int)Sol1[0].x] = punctActual;

                    int indiceUrmator = (indice+1)%N;
                    point punctUrmator= poly[poligonActual][indiceUrmator];
                    line segmentUrmator = line(0,0,0).dreapta2Puncte(punctActual, punctUrmator);
                    bool intreX =(punctActual.x<=punctDepartat.x && punctDepartat.x<=punctUrmator.x) || (punctActual.x>=punctDepartat.x && punctDepartat.x>=punctUrmator.x);
                    bool intreY =(punctActual.y<=punctDepartat.y && punctDepartat.y<=punctUrmator.y) || (punctActual.y>=punctDepartat.y && punctDepartat.y>=punctUrmator.y);
                    bool apartineLiniei = fabs(segmentUrmator.a*punctDepartat.x + segmentUrmator.b*punctDepartat.y + segmentUrmator.c) < epsilon;
                    if (intreX && intreY && apartineLiniei)
                    {
                        gasit = true;
                        Sol1[0].x++;
                        Sol1[(int)Sol1[0].x] = punctDepartat;
                        punctActual = punctDepartat;
                    }
                    indice = indiceUrmator;
                }
            }
            if (perimetruLocal <= perimetru[poligonActual]/2)
            {
                 //if (intalneste_segment(Sol[(int)Sol[0].x-1],Sol1[1]))
                    Sol[0].x++;
                 Sol[(int)Sol[0].x] = Sol1[1];
            }
            for (int i=2; i<=Sol1[0].x; i++)
            {
                Sol[0].x++;
                Sol[(int)Sol[0].x] = Sol1[i];
            }


        }
        else if (altul)
        {

            /// sari la alt poligon
            //if  (amFostPePoligon && !intalneste_segment(Sol[(int)Sol[0].x-1],punctPosibil))
                //Sol[0].x--;
            punctActual = punctPosibil;
            poligonActual = poligonPosibil;
            Sol[0].x++;
            Sol[(int)Sol[0].x] = punctActual;
            amFostPePoligon = false;

        }
        else
        {
            /// nu e nimic intre
            //if  (amFostPePoligon && !intalneste_segment(Sol[(int)Sol[0].x-1],p2))
                //Sol[0].x--;

            Sol[0].x++;
            Sol[(int)Sol[0].x] = p2;
            punctActual = p2;
            amFostPePoligon = false;
        }
    }
    return Sol;
}

point* calculeaza_distanta2(point p1, point p2)
{
    point Sol[10000];
    Sol[0].x = 1;
    Sol[1] = p1;

    point punctActual = p1;
    int poligonActual = 0;
    int segmentActual=0;
    while (fabs(punctActual.x-p2.x) > epsilon || fabs(punctActual.y - p2.y)>epsilon)
    {
        bool asta = false;
        bool altul = false;
        double distanta = 100000;
        int poligonPosibil;
        folositS[segmentActual] = true;
        point punctPosibil;
        point urmatorulPunctPeContur;
        for (int i=1; i<=numberSegments; i++)
        {
            line d1 = line(0,0,0).dreapta2Puncte(punctActual,p2);
            line d2 = line(0,0,0).dreapta2Puncte(S[i].a, S[i].b);
            point intersection = d1.intersectie(d2);
            bool apartineA = false;
            bool apartineB = false;
            if  (
                ((S[i].a.x<=intersection.x && intersection.x<=S[i].b.x) || (S[i].a.x>=intersection.x && intersection.x>=S[i].b.x))
                &&
                ((S[i].a.y<=intersection.y && intersection.y<=S[i].b.y) || (S[i].a.y>=intersection.y && intersection.y>=S[i].b.y))
            ) apartineB = true;
            if  (
                ((punctActual.x<=intersection.x && intersection.x<=p2.x) || (punctActual.x>=intersection.x && intersection.x>=p2.x))
                &&
                ((punctActual.y<=intersection.y && intersection.y<=p2.y) || (punctActual.y>=intersection.y && intersection.y>=p2.y))
                &&
                !(fabs(punctActual.x-intersection.x)<epsilon && fabs(punctActual.y-intersection.y)<epsilon)
            ) apartineA = true;
            if (fabs(d1.panta2() - d2.panta2())<epsilon && fabs(d1.panta() - d2.panta())<epsilon)
            {
                apartineA = true;
                apartineB = true;
                distanta = 0;
                punctPosibil = p2;
            }

            if (apartineA && apartineB)
            {
                if (S[i].father == poligonActual) asta = true;
                else if (!folositS[i])
                {
                    double dis = distance2(punctActual,intersection);

                    if (dis < distanta)
                    {
                        distanta = dis;
                        segmentActual = i;
                        punctPosibil = intersection;
                        poligonPosibil = S[i].father;
                    }
                    altul = true;
                }
            }
            bool intreX =(S[i].a.x<=punctActual.x && punctActual.x<=S[i].b.x) || (S[i].a.x>=punctActual.x && punctActual.x>=S[i].b.x);

            bool intreY =(S[i].a.y<=punctActual.y && punctActual.y<=S[i].b.y) || (S[i].a.y>=punctActual.y && punctActual.y>=S[i].b.y);
            bool capatA =fabs(punctActual.x-S[i].a.x)<epsilon && fabs(punctActual.y-S[i].a.y)<epsilon;
            line laturaPosibila = line(0,0,0).dreapta2Puncte(S[i].a, S[i].b);
            bool apartineLiniei = fabs(laturaPosibila.a*punctActual.x + laturaPosibila.b*punctActual.y + laturaPosibila.c) < epsilon;
            if  (intreX && intreY && !capatA && apartineLiniei  )
            {
                urmatorulPunctPeContur = S[i].a;
                segmentActual = i;
            }


        }
        if (asta)
        {
            /// mergi pe contur
            punctActual = urmatorulPunctPeContur;
            Sol[0].x++;
            Sol[(int)Sol[0].x] = punctActual;
        }
        else if (altul)
        {
            /// sari la alt poligon
            punctActual = punctPosibil;
            poligonActual = poligonPosibil;
            Sol[0].x++;
            Sol[(int)Sol[0].x] = punctActual;


        }
        else
        {
            /// nu e nimic intre
            Sol[0].x++;
            Sol[(int)Sol[0].x] = p2;
            punctActual = p2;
        }
    }
    return Sol;
}
void preprocesare_distante()
{
    for (int i=1; i<numberPoints; i++)
        for (int j=i+1; j<=numberPoints; j++)
        {
            point *drum;
            if (i==j)
            {
                D[i][j].path.push_back(P[i]);
                D[i][j].path.push_back(P[j]);
                D[i][j].dist = 0;
            }
            else
            {
                initializeazaFolositS();
                drum = calculeaza_distanta(P[i],P[j]);
                for (int k=1; k<(int)drum[0].x; k++)
                {
                    D[i][j].path.push_back(drum[k]);

                    D[i][j].dist += drum[k].distance(drum[k+1]);
                }
                if ((int)drum[0].x>1) D[i][j].path.push_back(drum[(int)drum[0].x]);

                for (int k=D[i][j].path.size()-1; k>=0; k--)
                    D[j][i].path.push_back( D[i][j].path[k] );
                D[j][i].dist = D[i][j].dist;
            }
        }
}

void clearDistante()
{
    for (int i=1; i<=numberPoints; i++)
        for (int j=1; j<=numberPoints; j++)
        {
            D[i][j].path.clear();
            D[i][j].dist = 0;
        }
}

void clean()
{
    fileIn.open(fisierIntrare, fstream::in);
    fileOut.open(fisierTranzitie, fstream::out);
    char c,c2;
    while (!fileIn.eof())
    {
        c2= c;
        fileIn>>c;
        if (c>='0' && c<='9' && c2==')') fileOut<<'\n';
        if (c!=' ') fileOut<<c;

    }
    fileOut<<'\n';
    fileIn.close();
    fileOut.close();
}

void citire()
{
    for (int i=0; i<=300; i++) perimetru[i] = 0;

    char s[1000000];
    char sx[1000],sy[1000];
    int lungimex, lungimey;
    fileIn.getline(s,1000000);
    int length = strlen(s);
    int iterator=0;

    for (j=0; j<=numberPoly; j++) poly[j].clear();
    numberPoints = 0;
    numberPoly = 1;

    while (s[++iterator]!=':');

    /// Citeste puncte
    while (s[iterator]!='#' && iterator<length)
    {
        lungimex=0;
        lungimey=0;
        /// citeste un punct
        iterator++; // sari :  sau ,
        iterator++; //sari (
        while (s[iterator]!=',')
        {
            sx[lungimex++]=s[iterator];
            sx[lungimex]='\0';
            iterator++;
        }
        iterator++; // sari ,
        while (s[iterator]!=')')
        {
            sy[lungimey++]=s[iterator];
            sy[lungimey]='\0';
            iterator++;
        }
        iterator++; // sari )
        numberPoints++;
        P[numberPoints] = point (atof(sx), atof(sy));

    }

    ///Citeste poligoane

    while (iterator<length)
    {
        lungimex=0;
        lungimey=0;
        iterator++; // sari # sau ,
        iterator++; //sari (
        while (s[iterator]!=',')
        {
            sx[lungimex++]=s[iterator];
            sx[lungimex]='\0';
            iterator++;
        }
        iterator++; // sari ,
        while (s[iterator]!=')')
        {
            sy[lungimey++]=s[iterator];
            sy[lungimey]='\0';
            iterator++;
        }
        iterator++; // sari )
        poly[numberPoly].push_back(point(atof(sx),atof(sy)));

        if (s[iterator] == ';') numberPoly++;
    }

}

void curata_output()
{
    fileIn.open(fisierIesire, fstream::in);
    fileOut.open("output.txt", fstream::out);
    char x;
    char s[2082999];
    int j;
    fileIn.getline(s, 2999999);
    fileIn.getline(s, 2999999);
    for(int i=1; i<=30; i++)
    {
        fileIn.getline(s, 2999999);
        long long len=strlen(s);
//       g<<len<<" ";
        if(i<=9) j=3;
        else j=4;
        while(j<len)
        {
            x=s[j];
            if ((x>='0'&&x<='9')||(x=='.')||(x=='-')) fileOut<<x;
            else if(x==' ') fileOut<<x;
            else if(x==';') fileOut<<" 123456 123456";
            j++;
        }
        fileOut<<'\n';
    }
    fileIn.close();
    fileOut.close();
}
