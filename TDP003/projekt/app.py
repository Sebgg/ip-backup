#! /usr/bin/env python3
from flask import *
import datalager as dl
import json
from copy import deepcopy
import logging
from logging.handlers import RotatingFileHandler

database = dl.load("database.json")
app = Flask(__name__)

handler = RotatingFileHandler('debug.log', maxBytes=10000, backupCount=5)
handler.setLevel(logging.INFO)
handler.setFormatter(logging.Formatter('[%(asctime)s] %(levelname)s in %(module)s: %(message)s'))
app.logger.setLevel(logging.INFO)
app.logger.addHandler(handler)
app.logger.info("Current systemversion: V.1.0.1")

@app.errorhandler(404)
def page_not_found(error):
    app.logger.error('A 404 occured.')
    return render_template('page_not_found.html', page_name="Uh oh")

@app.errorhandler(500)
def special_exception_handler(error):
    app.logger.error('Error in database, check above for more info')
    return render_template('no_database.html', page_name='Database error')


@app.route('/')
def index():
    global database
    projects = dl.get_latest_project(database)
    app.logger.info("Index page loaded with no errors")
    return render_template("index.html", page_name="Homepage", project=projects)

@app.route('/search', methods=["GET", "POST"])
def search():
    global database
    form = ['course_code', 'course_name', 'group_size', 'project-id']
    search=''
    searchfield = None
    sort_by = 'start_date'
    sort_order = 'desc'
    if request.method == "POST":
        search = request.form["search"]
        app.logger.info("Search requested on {}".format(search))
        searchfield = request.form.getlist("fields")
        if searchfield == []:
            searchfield = None
        sortby = request.form.get("sort")
        sortorder = request.form.get("order")
    app.logger.info("Search page loaded with no errors")
    result = dl.search(database, search=search, searchfield=searchfield, sort_by=sort_by, sort_order=sort_order)
    return render_template('list.html', page_name="Memesamling", result = result)

@app.route('/techniques')
def techniques():
    global database
    techniques = dl.get_technique_stats(database)
    app.logger.info("Techniques page loaded with no errors.")
    return render_template("techniques.html", page_name="Techniques", techniques=techniques, db=database)

@app.route('/project/<int:project_id>')
def project(project_id):
    global database
    project = dl.get_project(database, project_id)
    if project == None:
        app.logger.error('A 404 occured in project/{}.'.format(project_id))
        return render_template('page_not_found.html', page_name="Uh oh")
    app.logger.info("Project {} loaded with no errors".format(project["project_id"]))
    return render_template("project.html", project=project, page_name=project["project_name"])

@app.route('/list')
def list():
    global database
    return render_template("list.html", page_name="Memesamling", result = database)

if __name__ == "__main__":
    app.run()
