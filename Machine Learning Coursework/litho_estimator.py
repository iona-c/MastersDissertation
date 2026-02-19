import numpy as np
import pandas as pd
from sklearn.pipeline import Pipeline
# Add any other imports that you need here
from sklearn.impute import SimpleImputer
from sklearn.preprocessing import StandardScaler, OneHotEncoder
from sklearn.compose import ColumnTransformer
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import f1_score,accuracy_score
from sklearn.dummy import DummyClassifier
from sklearn.svm import SVC

# If you created custom transformers or helper functions, you can also add them to this file.

class LithoEstimator:
    '''Used to predict lithology in IODP wells. The signature (method name, argument and return types) for the strict minimum number of methods needed are already written for you below.
    Simply complete the methods following your own notebook results. You can also add more methods than provided below in order to keep your code clean.'''

    def __init__(self, path:str='data/log_data.csv') -> None:
        self.df, self.split = self.pre_format(path)
        self.X_train, self.X_test =self.get_Xs()
        self.y_train, self.y_test=self.get_ys()
        self.model =self.get_model()

        '''The path is a path to the training file. The default is the file I gave you.
        You want to create an X_train, X_test, y_train and y_test following the same principle as in your
        notebook. You also want to define and train your estimator as soon as you create it.
        
        I recommend creatubg the following instance variables in your __init__ method:
        self.X_train, self.X_test, self.y_train, self.y_test
        self.encoder - the label encoder for your categories
        self.model - the entire trained model pipeline

        Note that this class should not handle hyperparameter searching or feature selection - if you did those in your Part B 
        simply use your best estimators.
        
        '''

    def pre_format(self,path:str):
        df =pd.read_csv(path)
        df=df.drop_duplicates()
        df.sort_values(by='DEPTH_WMSF')
        size =df.shape
        split=int(size[0]*0.7)
        return df, split

    def x_test_score(self) -> float:
        '''Returns the F1 macro score of the X_test. This should be of type float.'''
        return f1_score(y_true=self.y_test,y_pred=self.y_pred, average='macro')

    def get_Xs(self) -> (pd.DataFrame, pd.DataFrame):
        '''Returns the X_train and X_test. This method is already written for you.'''
        x_df =self.df.drop(['lithology'],axis =1)
        s =self.split
        X_train =x_df.loc[:s]
        X_test =x_df.loc[s:]
        num =X_train._get_numeric_data()
        num[num<0]=None
        num_cols=X_train.select_dtypes(include =np.number).columns
        X_train[num_cols]=num
        X_train =X_train.drop(['HFK','HTHO','IDPH','DEPTH_WMSF'], axis=1)
        return X_train, X_test
    
    def get_ys(self) -> (pd.DataFrame, pd.DataFrame):
        '''Returns the y_train and y_test. This method is already written for you.'''
        y_df =self.df.loc[:,['lithology']]
        y_train =y_df.loc[:self.split]
        y_test =y_df.loc[self.split:]

        return y_train, y_test

    def predict(self, path_to_new_file:str='data/new_data.csv') -> np.array:
        '''Uses the trained algorithm to predict and return the predicted labels on an unseen file.
        The default file is the unknown_data.csv file in your data folder.
        
        Return a numpy array (the default for the "predict()" function of sklearn estimator)'''
        y_pred =self.model.predict(path_to_new_file)
        return y_pred

    def get_model(self) -> Pipeline:
        '''returns the entire trained pipeline, i.e. your model. 
        This will include the data preprocessor and the final estimator.'''
        num_cols =self.X_train.select_dtypes(include=np.number).columns
        num_pipe =Pipeline([('simple Imputer',SimpleImputer(strategy='mean')),('standard scaler',StandardScaler())])
        num_pipe.fit(self.X_train[num_cols])

        cat_col =self.X_train.select_dtypes(include=object).columns
        cat_pipe =Pipeline([('One Hot encoder',OneHotEncoder(handle_unknown='ignore'))])
        cat_pipe.fit(self.X_train[cat_col])

        preprocessor=ColumnTransformer([('number preprocess',num_pipe,num_cols),
                                ('catagory preprocess',cat_pipe,['munsel_color'])])
        xtrain_transformed =preprocessor.fit_transform(self.X_train)
        Xtest_transformed =preprocessor.transform(self.X_test)
        
        model =Pipeline([('preprocessor',preprocessor),('svc model',SVC(kernel='linear'))])
        model.fit(self.X_train,self.y_train)
        return model