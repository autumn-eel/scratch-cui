#include <bits/stdc++.h>
#include <boost/property_tree/json_parser.hpp>
#define rep(i,n)for(int i=0;i<(n);i++)
using namespace std;
typedef long long ll;

struct st {
	int ty;
	ll a;
	double b;
	string c;
};
struct node {
	node*n,*c;
	st d;
} dat[200000];
int n=0;

unordered_map<string,st>var;
unordered_map<string,vector<st>>ls;
unordered_map<string,vector<node*>>def;
unordered_map<string,unordered_map<string,int>>pa;
boost::property_tree::ptree t,script;
int timer;

#define SIZE 10000000
st pool[SIZE],*v[SIZE];
int g=SIZE;

st*construct() {
	if(g==0) {
		puts("error");
		exit(0);
	}
	return v[--g];
}
void destroy(st*p) {
	p->c.clear();
	v[g++]=p;
}
int type(string&s) {
	if(s.empty())return 2;
	int i=0;
	if(s[0]=='-')i++;
	if(i>=(int)s.size()||s[i]=='.')return 2;
	int p=-1;
	for(; i<(int)s.size(); i++) {
		if(s[i]=='.') {
			if(p!=-1)return 2;
			p=i;
		}
		else if(s[i]<'0'||s[i]>'9')return 2;
	}
	if(p!=-1||s.size()>11||(s.size()>=10&&abs(stoll(s))>2147483647))return 1;
	return 0;
}

st&run(node*,vector<st*>&);

st*ret;

st&f1(node*s,vector<st*>&param) {//+
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	if(l.ty==1||r.ty==1) {
		double d=(l.ty==0?l.a:l.b);
		double e=(r.ty==0?r.a:r.b);
		l.ty=1;
		l.b=d+e;
		destroy(&r);
		return l;
	}
	else {
		l.a+=r.a;
		destroy(&r);
		return l;
	}
}
st&f2(node*s,vector<st*>&param) {//-
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	if(l.ty==1||r.ty==1) {
		double d=(l.ty==0?l.a:l.b);
		double e=(r.ty==0?r.a:r.b);
		l.ty=1;
		l.b=d-e;
		destroy(&r);
		return l;
	}
	else {
		l.a-=r.a;
		destroy(&r);
		return l;
	}
}
st&f3(node*s,vector<st*>&param) { //*
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	if(l.ty==1||r.ty==1) {
		double d=(l.ty==0?l.a:l.b);
		double e=(r.ty==0?r.a:r.b);
		l.ty=1;
		l.b=d*e;
		destroy(&r);
		return l;
	}
	else {
		l.a*=r.a;
		destroy(&r);
		return l;
	}
}
st&f4(node*s,vector<st*>&param) { // /
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	if(l.ty==1||r.ty==1) {
		double d=(l.ty==0?l.a:l.b);
		double e=(r.ty==0?r.a:r.b);
		l.ty=1;
		l.b=d/e;
		destroy(&r);
		return l;
	}
	else {
		ll d=l.a,e=r.a;
		if(d%e==0) {
			l.a/=e;
			destroy(&r);
			return l;
		}
		else {
			l.ty=1;
			l.b=(double)d/(double)e;
			destroy(&r);
			return l;
		}
	}
}
st&f5(node*s,vector<st*>&param) { //%
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	l.a%=r.a;
	destroy(&r);
	return l;
}
st&f6(node*s,vector<st*>&param) { // =
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	if(l.ty==2||r.ty==2) {
		if(l.ty==2&&r.ty==2) {
			l.ty=2;
			l.c=(l.c==r.c?"T":"F");
			destroy(&r);
			return l;
		}
		string d,e;
		if(l.ty==0)d=to_string(l.a);
		else if(l.ty==1)d=to_string(l.b);
		else d=l.c;
		if(r.ty==0)e=to_string(r.a);
		else if(r.ty==1)e=to_string(r.b);
		else e=r.c;

		l.ty=2;
		l.c=(d==e?"T":"F");
		destroy(&r);
		return l;
	}
	else if(l.ty==1||r.ty==1) {
		double d=(l.ty==0?l.a:l.b);
		double e=(r.ty==0?r.a:r.b);
		l.ty=2;
		l.c=(d==e?"T":"F");
		destroy(&r);
		return l;
	}
	else {
		l.ty=2;
		l.c=(l.a==r.a?"T":"F");
		destroy(&r);
		return l;
	}
}
st&f7(node*s,vector<st*>&param) { //<
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	if(l.ty==2||r.ty==2) {
		if(l.ty==2&&r.ty==2) {
			l.ty=2;
			l.c=(l.c<r.c?"T":"F");
			destroy(&r);
			return l;
		}
		string d,e;
		if(l.ty==0)d=to_string(l.a);
		else if(l.ty==1)d=to_string(l.b);
		else d=l.c;
		if(r.ty==0)e=to_string(r.a);
		else if(r.ty==1)e=to_string(r.b);
		else e=r.c;

		l.ty=2;
		l.c=(d<e?"T":"F");
		destroy(&r);
		return l;
	}
	else if(l.ty==1||r.ty==1) {
		double d=(l.ty==0?l.a:l.b);
		double e=(r.ty==0?r.a:r.b);
		l.ty=2;
		l.c=(d<e?"T":"F");
		destroy(&r);
		return l;
	}
	else {
		l.ty=2;
		l.c=(l.a<r.a?"T":"F");
		destroy(&r);
		return l;
	}
}
st&f8(node*s,vector<st*>&param) { //>
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	if(l.ty==2||r.ty==2) {
		if(l.ty==2&&r.ty==2) {
			l.ty=2;
			l.c=(l.c>r.c?"T":"F");
			destroy(&r);
			return l;
		}
		string d,e;
		if(l.ty==0)d=to_string(l.a);
		else if(l.ty==1)d=to_string(l.b);
		else d=l.c;
		if(r.ty==0)e=to_string(r.a);
		else if(r.ty==1)e=to_string(r.b);
		else e=r.c;

		l.ty=2;
		l.c=(d>e?"T":"F");
		destroy(&r);
		return l;
	}
	else if(l.ty==1||r.ty==1) {
		double d=(l.ty==0?l.a:l.b);
		double e=(r.ty==0?r.a:r.b);
		l.ty=2;
		l.c=(d>e?"T":"F");
		destroy(&r);
		return l;
	}
	else {
		l.ty=2;
		l.c=(l.a>r.a?"T":"F");
		destroy(&r);
		return l;
	}
}
st&f9(node*s,vector<st*>&param) { //&
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	l.ty=2;
	l.c=(l.c=="T"&&r.c=="T"?"T":"F");
	destroy(&r);
	return l;
}
st&f10(node*s,vector<st*>&param) { //|
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	l.ty=2;
	l.c=(l.c=="T"||r.c=="T"?"T":"F");
	destroy(&r);
	return l;
}

st&f11(node*s,vector<st*>&param) { //append:toList:
	auto&b=run(s->n->c,param);
	auto&c=run(s->n->n->c,param);
	ls[c.c].push_back(b);
	destroy(&b);
	destroy(&c);
	return *ret;
}
st&f12(node*s,vector<st*>&param) { //call
	auto&b=run(s->n->c,param);
	vector<st*>v{&b};
	auto c=s->n->n;
	while(c) {
		st&ss=run(c->c,param);
		v.push_back(&ss);
		c=c->n;
	}
	for(auto&p:def[b.c]) {
		run(p,v);
	}
	destroy(&b);
	return *ret;
}
st&f13(node*s,vector<st*>&param) { //changeVar:by:
	auto&b=run(s->n->c,param);
	auto&c=run(s->n->n->c,param);
	if(var[b.c].ty==1) {
		if(c.ty==1)var[b.c].b+=c.b;
		else var[b.c].b+=c.a;
	}
	else {
		if(c.ty==1) {
			var[b.c].ty=1;
			var[b.c].b=var[b.c].a+c.b;
		}
		else {
			var[b.c].a+=c.a;
		}
	}
	destroy(&b);
	destroy(&c);
	return *ret;
}
st&f14(node*s,vector<st*>&param) { //computeFunction:of:
	auto&b=run(s->n->c,param);
	auto&c=run(s->n->n->c,param);
	if(b.c=="abs") {
		if(c.ty==0)c.a=abs(c.a);
		else if(c.ty==1)c.b=abs(c.b);
		destroy(&b);
		return c;
	}
	if(b.c=="floor") {
		if(c.ty==1)c.a=floor(c.b);
		c.ty=0;
		destroy(&b);
		return c;
	}
	return *ret;
}
st&f15(node*s,vector<st*>&param) { //concatenate:with:
	auto&l=run(s->n->c,param);
	auto&r=run(s->n->n->c,param);
	if(l.ty==0)l.c=to_string(l.a);
	else if(l.ty==1)l.c=to_string(l.b);
	if(r.ty==0)r.c=to_string(r.a);
	else if(r.ty==1)r.c=to_string(r.b);
	l.ty=2;
	l.c+=r.c;
	destroy(&r);
	return l;
}
st&f16(node*s,vector<st*>&param) { //deleteLine:ofList:
	auto&b=run(s->n->c,param);
	auto&c=run(s->n->n->c,param);
	if(b.ty==2) {
		if(b.c=="all")ls[c.c].clear();
		else if(b.c=="last")ls[c.c].pop_back();
	}
	else {
		ls[c.c].erase(ls[c.c].begin()+b.a-1);
	}
	destroy(&b);
	destroy(&c);
	return *ret;
}
st&f17(node*s,vector<st*>&param) { //doIf
	auto&b=run(s->n->c,param);
	if(b.c=="T") {
		auto c=s->n->n->c;
		while(c) {
			run(c->c,param);
			c=c->n;
		}
	}
	destroy(&b);
	return *ret;
}
st&f18(node*s,vector<st*>&param) {//doIfElse
	auto&b=run(s->n->c,param);
	if(b.c=="T") {
		auto c=s->n->n->c;
		while(c) {
			run(c->c,param);
			c=c->n;
		}
	}
	else {
		auto c=s->n->n->n->c;
		while(c) {
			run(c->c,param);
			c=c->n;
		}
	}
	destroy(&b);
	return *ret;
}
st&f19(node*s,vector<st*>&param) {//doRepeat
	auto&b=run(s->n->c,param);
	rep(i,b.a) {
		auto c=s->n->n->c;
		while(c) {
			run(c->c,param);
			c=c->n;
		}
	}
	destroy(&b);
	return *ret;
}
st&f20(node*s,vector<st*>&param) {//doUntil
	while(run(s->n->c,param).c=="F") {
		auto c=s->n->n->c;
		while(c) {
			run(c->c,param);
			c=c->n;
		}
	}
	return *ret;
}
st&f21(node*s,vector<st*>&param) {//getParam
	auto&b=run(s->n->c,param);
	b=*param[pa[param[0]->c][b.c]+1];
	return b;
}
st&f22(node*s,vector<st*>&param) {//getLine:ofList:
	auto&b=run(s->n->c,param);
	auto&c=run(s->n->n->c,param);
	if(b.ty==2) {
		if(b.c=="last") {
			return c=ls[c.c].back();
		}
		else if(b.c=="random") {
			random_device rnd;
			uniform_int_distribution<>rn(0,ls[c.c].size()-1);
			return c=ls[c.c][rn(rnd)];
		}
	}
	c=ls[c.c][b.a-1];
	destroy(&b);
	return *ret;
}
st&f23(node*s,vector<st*>&param) {//insert:at:ofList:
	auto&b=run(s->n->c,param);
	auto&c=run(s->n->n->c,param);
	auto&d=run(s->n->n->n->c,param);
	if(c.ty==2) {
		if(c.c=="last") {
			ls[d.c].push_back(b);
		}
		else if(c.c=="random") {
			random_device rnd;
			uniform_int_distribution<>rn(0,ls[d.c].size()-1);
			ls[d.c].insert(ls[d.c].begin()+rn(rnd),b);
		}
	}
	else {
		ls[d.c].insert(ls[d.c].begin()+c.a-1,b);
	}
	destroy(&b);
	destroy(&c);
	destroy(&d);
	return *ret;
}
st&f24(node*s,vector<st*>&param) {//letter:of:
	auto&b=run(s->n->c,param);
	auto&c=run(s->n->n->c,param);
	char e=c.c[b.a-1];
	c.c="";
	c.c+=e;
	destroy(&b);
	return c;
}
st&f25(node*s,vector<st*>&param) {//lineCountOfList:
	auto&b=run(s->n->c,param);
	b.ty=0;
	b.a=ls[b.c].size();
	b.c.clear();
	return b;
}
st&f26(node*s,vector<st*>&param) {//list:contains:
	auto&b=run(s->n->c,param);
	auto&c=run(s->n->n->c,param);
	bool flag=false;
	for(auto&i:ls[b.c]){
		if(i.ty==c.ty){
			if(i.ty==0&&i.a==c.a)flag=true;
			if(i.ty==1&&i.b==c.b)flag=true;
			if(i.ty==2&&i.c==c.c)flag=true;
		}	
	}
	b.c=(flag?"T":"F");
	destroy(&c);
	return b;
}
st&f28(node*s,vector<st*>&param) { //not
	auto&b=run(s->n->c,param);
	b.ty=2;
	b.c=(b.c=="T"?"F":"T");
	return b;
}
st&f27(node*s,vector<st*>&param){//randomFrom:to:
	auto&b=run(s->n->c,param);
	auto&c=run(s->n->n->c,param);
	random_device rnd;
	if(b.ty==1||c.ty==1){
		double d=(b.ty==0?b.a:b.b);
		double e=(c.ty==0?c.a:c.b);
		uniform_real_distribution<>rn(d,e);
		b.ty=1;b.b=rn(rnd);
	}
	else{
		uniform_int_distribution<>rn(b.a,c.a);
		b.a=rn(rnd);
	}
	destroy(&c);
	return b;
}
st&f29(node*s,vector<st*>&param) {//readVariable
	auto&b=run(s->n->c,param);
	b=var[b.c];
	return b;
}
st&f30(node*s,vector<st*>&param) {//rounded
	auto&b=run(s->n->c,param);
	if(b.ty==0)return b;
	b.b=round(b.b);
	return b;
}
st&f31(node*s,vector<st*>&param) {//setVar:to:
	auto&b=run(s->n->n->c,param);
	auto&c=run(s->n->c,param);
	if(b.ty==2) {
		b.ty=type(b.c);
		if(b.ty==0)b.a=stoll(b.c);
		else if(b.ty==1)b.b=stod(b.c);
	}
	var[c.c]=b;
	destroy(&b);
	destroy(&c);
	return *ret;
}
st&f32(node*s,vector<st*>&param) {//setLine:ofList:to:
	auto&b=run(s->n->c,param);
	auto&c=run(s->n->n->n->c,param);
	auto&d=run(s->n->n->c,param);
	ls[d.c][b.a-1]=c;
	destroy(&b);
	destroy(&c);
	destroy(&d);
	return *ret;
}
st&f33(node*s,vector<st*>&param) {//stopScripts
	exit(0);
}
st&f34(node*s,vector<st*>&param) {//stringLength:
	auto&b=run(s->n->c,param);
	b.ty=0;
	b.a=b.c.size();
	b.c.clear();
	return b;
}
st&f35(node*s,vector<st*>&param) {//timer
	ret=construct();
	ret->ty=1;
	ret->b=(clock()-timer)/(double)CLOCKS_PER_SEC;
	return *ret;
}
st&f36(node*s,vector<st*>&param) {//timerReset
	timer=clock();
	return *ret;
}

st&(*func[])(node*,vector<st*>&)= {f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36};
string funcname[]= {"+","-","*","/","%","=","<",">","&","|","append:toList:","call","changeVar:by:","computeFunction:of:","concatenate:with:","deleteLine:ofList:","doIf","doIfElse","doRepeat","doUntil","getParam","getLine:ofList:","insert:at:ofList:","letter:of:","lineCountOfList:","list:contains:","randomFrom:to:","not","readVariable","rounded","setVar:to:","setLine:ofList:to:","stopScripts","stringLength:","timer","timerReset"};

st&run(node*s,vector<st*>&param) {
	if(s->n==NULL) {
		ret=construct();
		return *ret=s->d;
	}
	return func[s->d.a](s,param);
}
void dfs(boost::property_tree::ptree&s) {
	if(s.empty()) {
		int ty=type(s.data());
		if(ty==0) {
			dat[n].d.ty=0;
			dat[n].d.a=stoll(s.data());
		}
		else if(ty==1) {
			dat[n].d.ty=1;
			dat[n].d.b=stod(s.data());
		}
		else {
			dat[n].d.ty=2;
			dat[n].d.c=s.data();
		}
		n++;
		return;
	}
	auto S=s.begin()->second.data();
	dat[n].d.a=find(begin(funcname),end(funcname),S)-begin(funcname);
	dat[n].n=&dat[n+1];
	n++;
	if(S=="doIf"||S=="doRepeat"||S=="doUntil"||S=="doIfElse") {
		int t=n;
		dat[n].c=&dat[n+1];
		n++;
		dfs((++s.begin())->second);
		dat[t].n=&dat[n];
		auto c=(++(++s.begin()))->second;
		int k=n;
		dat[n].c=&dat[n+1];
		n++;
		for(auto it=c.begin(); it!=c.end(); it++) {
			t=n;
			dat[n].c=&dat[n+1];
			n++;
			dfs(it->second);
			if(it!=--c.end())dat[t].n=&dat[n];
		}
		if(S=="doIfElse") {
			dat[k].n=&dat[n];
			dat[n].c=&dat[n+1];
			n++;
			c=(++(++(++s.begin())))->second;
			for(auto it=c.begin(); it!=c.end(); it++) {
				t=n;
				dat[n].c=&dat[n+1];
				n++;
				dfs(it->second);
				if(it!=--c.end())dat[t].n=&dat[n];
			}
		}
	}
	else {
		for(auto it=++s.begin(); it!=s.end(); it++) {
			int t=n;
			dat[n].c=&dat[n+1];
			n++;
			dfs(it->second);
			if(it!=--s.end())dat[t].n=&dat[n];
		}
	}
}

string project_path,input_path,output_path;

char b[100];
string p;

void unzip() {
	int l=readlink( "/proc/self/exe", b, sizeof(b));
	b[l]='\0';
	p=b;
	int i;
	for(i=p.size()-1; i>=0; i--) {
		if(p[i]=='/')break;
	}
	p=p.substr(0,i+1);
	system(("unzip -oq "+project_path+" -d"+p+"data").data());
}

int main(int argc,char**argv) {
	rep(i,SIZE)v[i]=&pool[i];
	project_path=argv[1];
	for(int i=2; i<argc; i++) {
		if(string(argv[i]).substr(0,5)=="--in=")input_path=string(argv[i]).substr(5);
		if(string(argv[i]).substr(0,6)=="--out=")output_path=string(argv[i]).substr(6);
	}
	unzip();
	read_json((p+"data/project.json").data(),t);
	for(auto&p:t) {
		if(p.first=="variables") {
			for(auto&q:p.second) {
				string name=q.second.get<string>("name");
				string value=q.second.get<string>("value");
				int ty=type(value);
				if(ty==0) {
					var[name].ty=0;
					var[name].a=stoll(value);
				}
				else if(ty==1) {
					var[name].ty=1;
					var[name].b=stod(value);
				}
				else {
					var[name].ty=2;
					var[name].c=value;
				}
			}
		}
		if(p.first=="lists") {
			for(auto&q:p.second) {
				string name=q.second.get<string>("listName");
				auto&u=q.second.get_child("contents");
				ls[name].reserve(2100000);
				for(auto&r:u) {
					string value=r.second.data();
					int ty=type(value);
					st ss;
					if(ty==0) {
						ss.ty=0;
						ss.a=stoll(value);
					}
					else if(ty==1) {
						ss.ty=1;
						ss.b=stod(value);
					}
					else {
						ss.ty=2;
						ss.c=value;
					}
					ls[name].push_back(ss);
				}
			}
		}
		if(p.first=="children") {
			for(auto&q:p.second) {
				if(q.second.count("scripts")) {
					script=q.second.get_child("scripts");
					break;
				}
			}
		}
	}
	if(input_path!="") {
		ls["input"].clear();
		input_path.erase(remove(input_path.begin(),input_path.end(),'\''),input_path.end());
		fstream f(input_path);
		string buf;
		while(getline(f,buf)) {
			if(buf.back()==(char)13)buf.pop_back();
			st ss;
			ss.ty=type(buf);
			if(ss.ty==0)ss.a=stoll(buf);
			else if(ss.ty==1)ss.b=stod(buf);
			else ss.c=buf;
			ls["input"].push_back(ss);
		}
	}
	vector<node*>root;
	for(auto it=script.begin(); it!=script.end(); it++) {
		auto&p=(++(++it->second.begin()))->second;
		bool flag=false,proc=false;
		string name;
		for(auto&q:p) {
			if(flag) {
				root.push_back(&dat[n]);
				dfs(q.second);
			}
			if(q.second.begin()->second.data()=="whenGreenFlag")flag=true;
			if(q.second.begin()->second.data()=="procDef")proc=true;
			if(proc) {
				if(name=="") {
					name=(++q.second.begin())->second.data();
					auto&r=(++(++q.second.begin()))->second;
					int c=0;
					for(auto&s:r) {
						pa[name][s.second.data()]=c++;
					}
				}
				else {
					def[name].push_back(&dat[n]);
					dfs(q.second);
				}
			}
		}
	}
	vector<st*>vv;
	for(auto&p:root) {
		run(p,vv);
	}
	if(output_path!="") {
		output_path.erase(remove(output_path.begin(),output_path.end(),'\''),output_path.end());
		ofstream f(output_path,ios::trunc);
		for(auto&p:ls["output"]) {
			if(p.ty==0)f<<p.a<<endl;
			else if(p.ty==1)f<<p.b<<endl;
			else f<<p.c<<endl;
		}
	}
	else {
		cout<<"output:"<<endl;
		for(auto&p:ls["output"]) {
			if(p.ty==0)cout<<p.a<<endl;
			if(p.ty==1)cout<<p.b<<endl;
			else cout<<p.c<<endl;
		}
	}
}
